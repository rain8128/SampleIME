#include <windows.h>
#include "globals.h"

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpvReserved){
    switch(fdwReason){
        case DLL_PROCESS_ATTACH:
            hInstance = hInst;
            break;
    }
    return TRUE;
}
