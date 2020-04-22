// decfile.cpp : Defines the entry point for the console application.
//
#include "windows.h"
// ToDo Allison recommends using bcrypt instead of wincrypt.
#include "wincrypt.h"
#include <stdio.h>

void LogFile(unsigned char* lpBuffer, DWORD buflen, LPCSTR fname);

int decryption()
{
	unsigned char buffer[1024];
	unsigned char password[] = "helloworld";
	DWORD dwDataLen;
	BOOL Final;

	DWORD buflen;
	char fname[] = "C:\\Users\\mitch\\Desktop\\Blush3ll\\src\\Loader-win\\Loader-win\\PAYLOADS\\poetry.encr";
	HANDLE hFile = CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(hFile, buffer, 1024, &buflen, NULL);
	CloseHandle(hFile);

	HCRYPTPROV hProv;

	if (CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		HCRYPTHASH hHash;
		if (CryptCreateHash(hProv, CALG_SHA_256, NULL, NULL, &hHash))
		{
			printf("Created hash.\n");
			if (CryptHashData(hHash, password, strlen((char*)password), NULL))
			{
				printf("Got hash data.\n");
				HCRYPTKEY hKey;
				if (CryptDeriveKey(hProv, CALG_AES_256, hHash, NULL, &hKey)) //changed from aes 128 to 256
				{
					printf("Derived key.\n");
					Final = true;
					dwDataLen = buflen;
					if (CryptDecrypt(hKey, NULL, Final, NULL, (unsigned char*)&buffer, &dwDataLen))
					{
						printf("decrypted message: %s\n", buffer);
						printf("saving decrypted message to message.dec");
						LogFile(buffer, dwDataLen, (char*)"message.dec");
					}
					printf("Crypto failed: %d\n", GetLastError());
					CryptDestroyKey(hKey);
				}
			}
			CryptDestroyHash(hHash);
		}
		CryptReleaseContext(hProv, 0);
	}
	return 0;
}

void LogFile(unsigned char* lpBuffer, DWORD buflen, LPCSTR fname) {

	BOOL bErrorFlag;
	DWORD dwBytesWritten;

	DeleteFileA(fname);

	HANDLE hFile = CreateFileA(fname, FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	bErrorFlag = WriteFile(hFile, lpBuffer, buflen, &dwBytesWritten, NULL);
	CloseHandle(hFile);

	Sleep(10);

	return;
}