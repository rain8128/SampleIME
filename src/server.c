#include <windows.h>
#include <ole2.h>
#include "register.h"

__declspec(dllexport) HRESULT DllRegisterServer(void){
    HRESULT hr;

    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    if(FAILED(hr)){
        return E_FAIL;
    }

    if(!(RegisterServer() && RegisterProfiles() && RegisterCategories())){
        return E_FAIL;
    }

    CoUninitialize();

    return S_OK;
}

__declspec(dllexport) HRESULT DllUnregisterServer(void){
    HRESULT hr;

    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    if(FAILED(hr)){
        return E_FAIL;
    }

    UnregisterProfiles();
    UnregisterCategories();
    UnregisterServer();

    CoUninitialize();

    return S_OK;
}
