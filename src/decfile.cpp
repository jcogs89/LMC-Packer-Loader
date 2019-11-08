// decfile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
// ToDo Allison recommends using bcrypt instead of wincrypt.
#include "wincrypt.h"

void LogFile(unsigned char* lpBuffer, DWORD buflen, LPCSTR fname);

int main()
{
	unsigned char buffer[1024];
	unsigned char password[] = "this0is0quite0a0long0cryptographic0key";
	DWORD dwDataLen;
	BOOL Final;

	DWORD buflen;
	char fname[] = "message.enc";
	HANDLE hFile = CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(hFile, buffer, 1024, &buflen, NULL);
	CloseHandle(hFile);

	HCRYPTPROV hProv;

	if (CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		HCRYPTHASH hHash;
		if (CryptCreateHash(hProv, CALG_SHA_256, NULL, NULL, &hHash))
		{
			if (CryptHashData(hHash, password, strlen((char*)password), NULL))
			{
				HCRYPTKEY hKey;
				if (CryptDeriveKey(hProv, CALG_AES_128, hHash, NULL, &hKey))
				{
					Final = true;
					dwDataLen = buflen;
					if ( CryptDecrypt(hKey, NULL, Final, NULL, (unsigned char*)&buffer, &dwDataLen) )
					{
						printf("decrypted message: %s\n", buffer);
						printf("saving decrypted message to message.dec");
						LogFile(buffer, dwDataLen, (char*)"message.dec");
					}
					printf("%d\n", GetLastError());
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