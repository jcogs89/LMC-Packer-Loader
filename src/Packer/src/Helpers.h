
/*
 * Helpers.h
 *
 *  Created on: Feb 27, 2020
 *      Author: cbai
 */
#include "modes.h"
#include "aes.h"
#include "filters.h"
using namespace std;

int intinput();
int ziphelp(string in, string out);
int uziphelp(string in, string out);
int encrypthelp(string in, string out);
int decrypthelp(string in, string out);
int Hash(std::string msg, CryptoPP::byte* digest);
string Encrypt(string plain_in, int size, CryptoPP::byte key[32], CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE]);
string Decrypt(string cipher_in, int size, CryptoPP::byte key[32], CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE]);

