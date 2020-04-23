// decfile.cpp : Defines the entry point for the console application.
//
#include "windows.h"
// ToDo Allison recommends using bcrypt instead of wincrypt.
#include "wincrypt.h"
#include <stdio.h>

void LogFile(unsigned char* lpBuffer, DWORD buflen, LPCSTR fname);

int decryption()
{
	printf("Todo- implement decryption");
}