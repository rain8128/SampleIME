#include <windows.h>
#include <msctf.h>
#include <winreg.h>
#include <string.h>
#include "globals.h"

const WCHAR ServiceName[] = L"SampleIME";

BOOL RegisterServer(void){

    WCHAR *lpchCLSID;
    StringFromCLSID(&CLSID_SampleIME, &lpchCLSID);
    WCHAR awchInfoKey[64] = L"CLSID\\";
    wcscat(awchInfoKey, lpchCLSID);

    HKEY hInfoKey;
    HKEY hSubKey;
    WCHAR FileName[MAX_PATH];

    BOOL fRet = TRUE;

    if(fRet &= (RegCreateKeyExW(
        HKEY_CLASSES_ROOT,
        awchInfoKey,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE,
        NULL,
        &hInfoKey,
        NULL
    ) == ERROR_SUCCESS)){
        fRet &= (RegSetValueExW(
            hInfoKey,
            NULL,
            0,
            REG_SZ,
            (BYTE*)ServiceName,
            sizeof(ServiceName)    
        ) == ERROR_SUCCESS);
        if(fRet &= (RegCreateKeyExW(
            hInfoKey,
            L"InProcServer32",
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_WRITE,
            NULL,
            &hSubKey,
            NULL
        ) == ERROR_SUCCESS)){
            DWORD strlength = GetModuleFileNameW(hInstance, FileName, sizeof(FileName));

            fRet &= (RegSetValueExW(
                hSubKey,
                NULL,
                0,
                REG_SZ,
                (BYTE*)FileName,
                strlength * sizeof(WCHAR)
            ) == ERROR_SUCCESS);
            fRet &= (RegSetValueExW(
                hSubKey,
                L"ThreadingModel",
                0,
                REG_SZ,
                (BYTE*)L"Apartment",
                _countof(L"Apartment")*sizeof(WCHAR)
            ) == ERROR_SUCCESS);
            RegCloseKey(hSubKey);
        }
        RegCloseKey(hInfoKey);
    }

    return fRet;
}

BOOL UnregisterServer(void){

    WCHAR *lpchCLSID;
    StringFromCLSID(&CLSID_SampleIME, &lpchCLSID);
    WCHAR awchInfoKey[64] = L"CLSID\\";
    wcscat(awchInfoKey, lpchCLSID);

    HKEY hInfoKey;

    BOOL fRet = TRUE;

    if((RegOpenKeyExW(
        HKEY_CLASSES_ROOT,
        awchInfoKey,
        0,
        KEY_ALL_ACCESS,
        &hInfoKey
    ) == ERROR_SUCCESS)){
        fRet &= (RegDeleteKeyW(
            hInfoKey,
            L"InProcServer32"
        ) == ERROR_SUCCESS);
        RegCloseKey(hInfoKey);
    }else{
        return TRUE;
    }


    fRet &= (RegDeleteKeyW(
        HKEY_CLASSES_ROOT,
        awchInfoKey
    ) == ERROR_SUCCESS);

    return fRet;
}

BOOL RegisterProfiles(void){
    HRESULT hr;

    ITfInputProcessorProfiles *pInputProcessorProfiles;

    hr = CoCreateInstance( 
        &CLSID_TF_InputProcessorProfiles,
        NULL,
        CLSCTX_INPROC_SERVER,
        &IID_ITfInputProcessorProfiles,
        (LPVOID*)&pInputProcessorProfiles
    );

    if(FAILED(hr)){
        return FALSE;
    }

    hr = pInputProcessorProfiles->lpVtbl->Register(
        pInputProcessorProfiles,
        &CLSID_SampleIME
    );
    
    if(FAILED(hr)){
        return FALSE;
    }

    hr = pInputProcessorProfiles->lpVtbl->AddLanguageProfile(
        pInputProcessorProfiles,
        &CLSID_SampleIME,
        0x411, //ja-JP
        &GUID_SampleIMEProfile,
        ServiceName,
        -1,
        NULL,
        0,
        0
    );

    pInputProcessorProfiles->lpVtbl->Release(pInputProcessorProfiles);

    return SUCCEEDED(hr);
}

BOOL UnregisterProfiles(void){
    HRESULT hr;

    ITfInputProcessorProfiles *pInputProcessorProfiles;

    hr = CoCreateInstance( 
        &CLSID_TF_InputProcessorProfiles,
        NULL,
        CLSCTX_INPROC_SERVER,
        &IID_ITfInputProcessorProfiles,
        (LPVOID*)&pInputProcessorProfiles
    );

    if(FAILED(hr)){
        return FALSE;
    }

    hr = pInputProcessorProfiles->lpVtbl->RemoveLanguageProfile(
        pInputProcessorProfiles,
        &CLSID_SampleIME,
        0x411, //ja-JP
        &GUID_SampleIMEProfile
    );
    
    if(FAILED(hr)){
        return FALSE;
    }
    
    hr = pInputProcessorProfiles->lpVtbl->Unregister(
        pInputProcessorProfiles,
        &CLSID_SampleIME
    );
    
    pInputProcessorProfiles->lpVtbl->Release(pInputProcessorProfiles);

    return SUCCEEDED(hr);
}

BOOL RegisterCategories(void){
    HRESULT hr;

    ITfCategoryMgr *pCategoryMgr;

    hr = CoCreateInstance(
        &CLSID_TF_CategoryMgr,
        NULL,
        CLSCTX_INPROC_SERVER,
        &IID_ITfCategoryMgr,
        (LPVOID*)&pCategoryMgr
    );

    if(FAILED(hr)){
        return FALSE;
    }

    hr = pCategoryMgr->lpVtbl->RegisterCategory(
        pCategoryMgr,
        &CLSID_SampleIME,
        &GUID_TFCAT_TIP_KEYBOARD,
        &CLSID_SampleIME
    );

    pCategoryMgr->lpVtbl->Release(pCategoryMgr);

    return SUCCEEDED(hr);
}

BOOL UnregisterCategories(void){
    HRESULT hr;

    ITfCategoryMgr *pCategoryMgr;

    hr = CoCreateInstance(
        &CLSID_TF_CategoryMgr,
        NULL,
        CLSCTX_INPROC_SERVER,
        &IID_ITfCategoryMgr,
        (LPVOID*)&pCategoryMgr
    );

    if(FAILED(hr)){
        return FALSE;
    }

    hr = pCategoryMgr->lpVtbl->UnregisterCategory(
        pCategoryMgr,
        &CLSID_SampleIME,
        &GUID_TFCAT_TIP_KEYBOARD,
        &CLSID_SampleIME
    );
    
    pCategoryMgr->lpVtbl->Release(pCategoryMgr);

    return SUCCEEDED(hr);
}
