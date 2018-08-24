#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>

#define ARG_SIZE 3
#define LOG(info) printf("\n[ * ] --> "); \
		printf info

#define DLL_PATH_INDEX 1
#define PROCESS_ID_INDEX 2

int main(int argc, char* argv[])
{
	if (argc != ARG_SIZE)
	{
		printf("Usage: Injector.exe <dll path> <process id>\n");
		return 1;
	}

	LPCSTR DllPath = argv[DLL_PATH_INDEX];
	DWORD ProcessId = atoi(argv[PROCESS_ID_INDEX]);

	LOG(("Injecting the dll: %s to process id: %d", DllPath, ProcessId));
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);

	// Allocate memory for the dllpath in the target process
	// length of the path string + null terminator
	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(DllPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);

	// Write the path to the address of the memory we just allocated
	// in the target process
	WriteProcessMemory(hProcess, pDllPath, (LPVOID)DllPath,
		strlen(DllPath) + 1, 0);

	// Create a Remote Thread in the target process which
	// calls LoadLibraryA as our dllpath as an argument -> program loads our dll
	HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),
			"LoadLibraryA"), pDllPath, 0, 0);

	// Wait for the execution of our loader thread to finish
	WaitForSingleObject(hLoadThread, INFINITE);

	LOG(("Dll Hooked: %x\n", pDllPath));
	system("pause");
	// Free the memory allocated for our dll path
	VirtualFreeEx(hProcess, pDllPath, strlen(DllPath) + 1, MEM_RELEASE);
}
