/*
 * Helpers.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: cbai
 */
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <assert.h>
#include <limits.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

#include "miniz/miniz.h"
#include "modes.h"
#include "aes.h"
#include "filters.h"
#include "sha3.h"
#include "hex.h"

using namespace std;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;

#define my_max(a,b) (((a) > (b)) ? (a) : (b))
#define my_min(a,b) (((a) < (b)) ? (a) : (b))

#define BUF_SIZE (1024 * 1024)
static uint8 s_inbuf[BUF_SIZE];
static uint8 s_outbuf[BUF_SIZE];


int intinput()
{
	string c;
	while (1)
	{
		cin >>c;
		try
		{
			return stoi(c);
		}
		catch (...)
		{
			printf("non numeric input\n>> ");
		}
	}
	return -1;
}

int ziphelp(string in, string out)
{
	//const char *pMode;
	FILE *pInfile, *pOutfile;
	uint infile_size;
	int level = Z_BEST_COMPRESSION;
	z_stream stream;
	//int p = 1;
	const char *pSrc_filename = in.c_str();
	const char *pDst_filename = out.c_str();
	long file_loc;

	  //printf("miniz.c version: %s\n", MZ_VERSION);

	  // Open input file.
	    pInfile = fopen(pSrc_filename, "rb");
	    if (!pInfile)
	    {
	      printf("Failed opening input file!\n");
	      return EXIT_FAILURE;
	    }

	  //printf("Mode: c, Level: %u\nInput File: \"%s\"\nOutput File: \"%s\"\n", level, pSrc_filename, pDst_filename);


	  // Determine input file's size.
	    fseek(pInfile, 0, SEEK_END);
	    file_loc = ftell(pInfile);
	    fseek(pInfile, 0, SEEK_SET);

	    if ((file_loc < 0) || (file_loc > INT_MAX))
	    {
	       // This is not a limitation of miniz or tinfl, but this example.
	       printf("File is too large to be processed by this example.\n");
	       return EXIT_FAILURE;
	    }

	    infile_size = (uint)file_loc;

	      // Open output file.
	      pOutfile = fopen(pDst_filename, "wb");
	      if (!pOutfile)
	      {
	        printf("Failed opening output file!\n");
	        return EXIT_FAILURE;
	      }

	     //printf("Input file size: %u\n", infile_size);

	      // Init the z_stream
	      memset(&stream, 0, sizeof(stream));
	      stream.next_in = s_inbuf;
	      stream.avail_in = 0;
	      stream.next_out = s_outbuf;
	      stream.avail_out = BUF_SIZE;

	      // Compression.
	          uint infile_remaining = infile_size;

	          if (deflateInit(&stream, level) != Z_OK)
	          {
	            printf("deflateInit() failed!\n");
	            return EXIT_FAILURE;
	          }

	          for ( ; ; ) {
	            int status;

	            // Input buffer is empty, so read more bytes from input file.
	            if (!stream.avail_in) {
	              uint n = my_min(BUF_SIZE, infile_remaining);

	              if (fread(s_inbuf, 1, n, pInfile) != n) {
	                printf("Failed reading from input file!\n");
	                return EXIT_FAILURE;
	              }

	              stream.next_in = s_inbuf;
	              stream.avail_in = n;

	              infile_remaining -= n;
	              //printf("Input bytes remaining: %u\n", infile_remaining);
	            }

	            status = deflate(&stream, infile_remaining ? Z_NO_FLUSH : Z_FINISH);

	            // Output buffer is full, or compression is done, so write buffer to output file.
	            if ((status == Z_STREAM_END) || (!stream.avail_out)) {
	              uint n = BUF_SIZE - stream.avail_out;

	              if (fwrite(s_outbuf, 1, n, pOutfile) != n) {
	                printf("Failed writing to output file!\n");
	                return EXIT_FAILURE;
	              }

	              stream.next_out = s_outbuf;
	              stream.avail_out = BUF_SIZE;
	            }

	            if (status == Z_STREAM_END)
	              break;
	            else if (status != Z_OK) {
	              printf("deflate() failed with status %i!\n", status);
	              return EXIT_FAILURE;
	            }
	          }

	          if (deflateEnd(&stream) != Z_OK)
	          {
	            printf("deflateEnd() failed!\n");
	            return EXIT_FAILURE;
	          }

		  fclose(pInfile);
		    if (EOF == fclose(pOutfile))
		    {
		      printf("Failed writing to output file!\n");
		      return EXIT_FAILURE;
		    }

		    //printf("Total input bytes: %u\n", (mz_uint32)stream.total_in);
		    //printf("Total output bytes: %u\n", (mz_uint32)stream.total_out);
		    //printf("Success: %d\n", EXIT_SUCCESS);
		    return EXIT_SUCCESS;
	    return -69;
}


int uziphelp(string in, string out)
{
	//const char *pMode;
		FILE *pInfile, *pOutfile;
		uint infile_size;
		int level = Z_BEST_COMPRESSION;
		z_stream stream;
		//int p = 1;
		const char *pSrc_filename = in.c_str();
		const char *pDst_filename = out.c_str();
		long file_loc;
		//printf("setsetstsetsetestestsetset\n");

		  printf("miniz.c version: %s\n", MZ_VERSION);

		  // Open input file.
		    pInfile = fopen(pSrc_filename, "rb");
		    if (!pInfile)
		    {
		      printf("Failed opening input file!\n");
		      return EXIT_FAILURE;
		    }

		  printf("Mode: c, Level: %u\nInput File: \"%s\"\nOutput File: \"%s\"\n", level, pSrc_filename, pDst_filename);


		  // Determine input file's size.
		    fseek(pInfile, 0, SEEK_END);
		    file_loc = ftell(pInfile);
		    fseek(pInfile, 0, SEEK_SET);

		    if ((file_loc < 0) || (file_loc > INT_MAX))
		    {
		       // This is not a limitation of miniz or tinfl, but this example.
		       printf("File is too large to be processed by this example.\n");
		       return EXIT_FAILURE;
		    }

		    infile_size = (uint)file_loc;

		      // Open output file.
		      pOutfile = fopen(pDst_filename, "wb");
		      if (!pOutfile)
		      {
		        printf("Failed opening output file!\n");
		        return EXIT_FAILURE;
		      }

		      printf("Input file size: %u\n", infile_size);

		      // Init the z_stream
		      memset(&stream, 0, sizeof(stream));
		      stream.next_in = s_inbuf;
		      stream.avail_in = 0;
		      stream.next_out = s_outbuf;
		      stream.avail_out = BUF_SIZE;





		      // Decompression.
		      uint infile_remaining = infile_size;

		      if (inflateInit(&stream))
		      {
		        printf("inflateInit() failed!\n");
		        return EXIT_FAILURE;
		      }

		      for ( ; ; )
		      {
		        int status;
		        if (!stream.avail_in)
		        {
		          // Input buffer is empty, so read more bytes from input file.
		          uint n = my_min(BUF_SIZE, infile_remaining);

		          if (fread(s_inbuf, 1, n, pInfile) != n)
		          {
		            printf("Failed reading from input file!\n");
		            return EXIT_FAILURE;
		          }

		          stream.next_in = s_inbuf;
		          stream.avail_in = n;

		          infile_remaining -= n;
		        }

		        status = inflate(&stream, Z_SYNC_FLUSH);

		        if ((status == Z_STREAM_END) || (!stream.avail_out))
		        {
		          // Output buffer is full, or decompression is done, so write buffer to output file.
		          uint n = BUF_SIZE - stream.avail_out;
		          if (fwrite(s_outbuf, 1, n, pOutfile) != n)
		          {
		            printf("Failed writing to output file!\n");
		            return EXIT_FAILURE;
		          }
		          stream.next_out = s_outbuf;
		          stream.avail_out = BUF_SIZE;
		        }

		        if (status == Z_STREAM_END)
		          break;
		        else if (status != Z_OK)
		        {
		          printf("inflate() failed with status %i!\n", status);
		          return EXIT_FAILURE;
		        }
		      }

		      if (inflateEnd(&stream) != Z_OK)
		      {
		        printf("inflateEnd() failed!\n");
		        return EXIT_FAILURE;
		      }







		      fclose(pInfile);
		        if (EOF == fclose(pOutfile))
		        {
		          printf("Failed writing to output file!\n");
		          return EXIT_FAILURE;
		        }

		        printf("Total input bytes: %u\n", (mz_uint32)stream.total_in);
		        printf("Total output bytes: %u\n", (mz_uint32)stream.total_out);
		        printf("Success.\n");
		        return EXIT_SUCCESS;
}


/*int encrypthelp(string in, string out)
{
	//printf("Encryption the file\n.");
	string cmd("openssl enc -aes-256-cbc -p -nosalt -in ");
	cmd += in;
	cmd += " -base64 -md sha1 -out ";
	cmd += out;
	printf("Openssl string ran: ", cmd.c_str());
	system(cmd.c_str());

	return -69;
}*/
int Hash(std::string msg, CryptoPP::byte* digest){
    CryptoPP::SHA3_256().CalculateDigest(digest, (const CryptoPP::byte*)msg.data(), msg.length());

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach( new CryptoPP::StringSink( output ) );
    encoder.Put( digest, 32 );
    encoder.MessageEnd();
    std::cout << output << std::endl;

    return 0;
}


string Encrypt(string plain_in, int size, CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE])
	{

	//Key and IV setup
	//AES encryption uses a secret key of a variable length (128-bit, 196-bit or 256-
	//bit). This key is secretly exchanged between two parties before communication
	//begins. DEFAULT_KEYLENGTH= 16 bytes

	//
	// String and Sink setup
	//
	string plaintext = plain_in;
	string ciphertext;

	//
	// Dump Plain Text
	//
	cout << "Plain Text (" << plaintext.size() << " bytes)" << endl;
	cout << plaintext;
	cout << endl << endl;

	//
	// Create Cipher Text
	//
	CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length());
	stfEncryptor.MessageEnd();

	//
	// Dump Cipher Text
	//
	cout << "Cipher Text (" << ciphertext.size() << " bytes)" << endl;

	for (unsigned i = 0; i < ciphertext.size(); i++) {

		cout << "0x" << hex << (0xFF & static_cast<CryptoPP::byte>(ciphertext[i])) << " ";
	}

	cout << endl << endl;

	return ciphertext;
}

string Decrypt(string cipher_in, int size, CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE]){


	string ciphertext = cipher_in;
	string decryptedtext;

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
	cout << "Decrypted Text (" << decryptedtext.size() << " bytes)" << endl;
	cout << "Decrypted Text: " << endl;
	cout << decryptedtext;
	cout << endl << endl;

	return decryptedtext;
}

int decrypthelp(string in, string out)
{
	return -69;
}
