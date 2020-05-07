#include <iostream>

#include "crypto/modes.h"
#include "crypto/aes.h"
#include "crypto/filters.h"
#include "crypto/sha3.h"
#include "crypto/hex.h"

using namespace std;

int Hash(std::string msg, CryptoPP::byte* digest) {
    CryptoPP::SHA3_256().CalculateDigest(digest, (const CryptoPP::byte*)msg.data(), msg.length());

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest, 32);
    encoder.MessageEnd();
    std::cout << output << std::endl;

    return 0;
}


string Decrypt(string ciphertext, int size, std::string password) {
    CryptoPP::byte key[32], iv[CryptoPP::AES::BLOCKSIZE];
    memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
    Hash(password, key);
    printf("got hash\n");
    string decryptedtext;
    
    //printf("ciphertext: %s\n", ciphertext);
    //printf("password: %s\n", password.c_str());

    //
    // Decrypt
    //
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);
    //printf("mid\n");
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedtext));
    //printf("1\n");
    stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
    //printf("2\n");
    //printf("Size of file: %d\n", ciphertext.size());
    stfDecryptor.MessageEnd();
    //printf("got decryption\n");
    //
    // Dump Decrypted Text
    //
    //cout << "Decrypted Text (" << ciphertext.size() << " bytes)" << endl;
    //cout << "Decrypted Text: " << endl;
    //cout << decryptedtext;
    //cout << endl << endl;

    return decryptedtext;
}
