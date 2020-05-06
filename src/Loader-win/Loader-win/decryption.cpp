#include <iostream>
#include <iomanip>

#include "crypto/modes.h"
#include "crypto/aes.h"
#include "crypto/filters.h"


std::string Decrypt(std::string cipher_in, CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE]) {


    std::string ciphertext = cipher_in;
    std::string decryptedtext;

    //
    // Decrypt
    //
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedtext));
    stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
    stfDecryptor.MessageEnd();

    //
    // Dump Decrypted Text
    //
    std::cout << "Decrypted Text: " << std::endl;
    std::cout << decryptedtext;
    std::cout << std::endl << std::endl;

    return decryptedtext;
}

//Decryption call
//string decryption_outp = Decrypt(encryption_outp, key, iv)
