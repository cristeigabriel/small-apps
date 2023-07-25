#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstring>

int main(int argc, char** argv)
{
	const DWORD dwPid = atoi(argv[1]);

	const HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL | TH32CS_SNAPMODULE32, dwPid);
	if (NULL == hSnapshot)
		return 0;

	const UINT_PTR dwAddress = strtoull(argv[2], nullptr, 16);

	MODULEENTRY32W me = {.dwSize = sizeof MODULEENTRY32W};
	BOOL bGoing = Module32FirstW(hSnapshot, &me);
	while (bGoing) {
		// If the address is in range then print module and stop
		if ((dwAddress >= (UINT_PTR)me.modBaseAddr) && (dwAddress <= ((UINT_PTR)me.modBaseAddr + (UINT_PTR)me.modBaseSize))) {
			std::wcout << L"Module is: " << me.szModule << L'\n';
			break;
		}

		bGoing = Module32NextW(hSnapshot, &me);
	}

	// We ran out of modules during the search
	if (FALSE == bGoing)
		std::wcout << L"Couldn't find module!\n";
	
	CloseHandle(hSnapshot);

	return 1;
}
