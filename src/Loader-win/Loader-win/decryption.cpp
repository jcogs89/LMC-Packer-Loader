// decfile.cpp : Defines the entry point for the console application.
//
#include "windows.h"
// ToDo Allison recommends using bcrypt instead of wincrypt.
#include "wincrypt.h"
#include <stdio.h>

void LogFile(unsigned char* lpBuffer, DWORD buflen, LPCSTR fname);

typedef struct payloadStruct
{
    unsigned int uncompressedLength;
    unsigned int encryptedLength;
    unsigned int decryptedLength;
    char** argv;
    char** envp;
} PayloadStruct;

unsigned char* decrypt(unsigned char* encrypted, unsigned int input_length, unsigned int original_size, unsigned char* key)
{
    unsigned char encrypted_msg[input_length - 24];
    unsigned char nonce[24];

    const unsigned int ciphertext_len = original_size + 16;

    unsigned char* decrypted = calloc(original_size, sizeof(unsigned char));

    if (sodium_init() == -1)
    {
        return NULL;
    }

    for (int i = 0; i < input_length; i++)
    {
        if (i > 23)
        {
            encrypted_msg[i - 24] = encrypted[i];
        }
        else
        {
            nonce[i] = encrypted[i];
        }
    }

    if (crypto_secretbox_open_easy(decrypted, encrypted_msg, ciphertext_len, nonce, key) != 0)
    {
        return NULL;
    }

    return decrypted;
}