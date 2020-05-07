#include "MemLoadLibrary.h"
#include <stdio.h>
typedef void(_cdecl* func)();

int exe_dll_in_mem(unsigned char *rawData, int size)
{
	printf("attempting to exec dll in memory...\n");
	CLoad lib;
	HANDLE hLibrary = lib.LoadFromMemory(rawData, size);
	func fn = (func)lib.GetProcAddressFromMemory(hLibrary, "DllMain");
	fn();
	lib.FreeLibraryFromMemory(hLibrary);
	return 0;
}