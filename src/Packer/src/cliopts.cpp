#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "colors.h"
#include "dirlist.h"
#include "Helpers.h"
#include "incom.h"
#include "cliopts.h"
#include "udpstuff.h"
#include "colors.h"
#include "modes.h"
#include "aes.h"
#include "filters.h"
#include "sha3.h"
#include "cryptlib.h"
using namespace std;

void listpayloads(std::vector < std::string > files) {
	std::cout << GREEN("Current payloads:\n");
	dirprint(files,11); //2 or 11 is magic, trust me //ToDo, whoever wrote this - please clarify.
}

void addpayload(std::string pathpacked, std::string pathstaging) {
	std::string ans, inp;
	int compression_ret;
	unsigned int id;
	std::vector<std::string> stage = dirlist(pathstaging);

	while (1) { //Take user input to select file to turn into a payload
		std::cout << GREEN("The following payloads are available in the staging folder:\n");
		dirprint(stage, 10); //10 is magic, trust me  //ToDo, whoever wrote this - please clarify.
		std::cout << GREEN("Enter the number for file to be packed ('x' to back out):");
		printf(YELLOW(">"));
		std::cin >> inp;
		clrscr();
		try {id = std::stoi(inp);
		if (id>stage.size()-1) {
				std::cout << RED("Invalid option, please try again.");
				continue;
			} else {
				break;
			}
		} catch (...) {
			if ((inp != "x") and (inp != "X")) {
				printf(RED("Unrecognized input, please try again."));
			} else {
				clrscr();
				return;
			}
		}
	}
	std::cout << "File: \"" << stage[id] << "\" selected.\n";

	std::cout << "Compressing File...\n";
	std::string compression_outp= pathpacked+stage[id].substr(10)+".zips";
	std::string iput = stage[id];
	compression_ret = ziphelp(iput, compression_outp);
	if (compression_ret == 0) { //Compression EXIT_SUCCESS is 0
		std::cout << GREEN("File Zipped Successfully!\n");
	} else {
		std::cout << RED("FILE ZIP FAILED");
	}


	/*//ENCRYPTION <><>
	string encryption_outp= pathpacked+stage[id].substr(10)+".encr";
	std::string encryption_outp= pathpacked+stage[id].substr(10)+".encr";
	encrypthelp(compression_outp, encryption_outp);
	std::cout << GREEN("\nFile Encrypted Successfully!\n");
	*/


	/////////////////////////
	//Hardcoded Key and IV//
	////////////////////////
	CryptoPP::byte key[32], iv[CryptoPP::AES::BLOCKSIZE];
	//memset(key, 0x00, 32);
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

	//prompt user to input password to encrypt
	string msg;

	cout << "Enter Password for Encryption: ";
	cin >> msg;


	//Hashing and key generation
	Hash(msg,key);


	//Encryption
	string encryption_outp = Encrypt(compression_outp, key, iv);


	//Decryption
	string decryption_outp = Decrypt(encryption_outp, key, iv);

	while (1) {
	        printf("Do you want to delete the source in staging (y/n)?");
	        printf(YELLOW(">"));
	        std::cin >> ans;
	        clrscr();
	        if (ans=="Y" or ans == "y") {
	            if(remove(iput.c_str()) != 0 ){perror( "Error deleting file" );}
	            printf(GREEN("File successfully deleted"));
	            break;}
	        else if (ans=="n" or ans == "N") {break;}
	        else {std::cout << RED("Invalid Option\n");}
	    }

	}

void sendpayload(std::string pathpacked) {
	std::vector<std::string> target;
	target = findnew("Known_hosts");
	if(target[0]=="null"){
		clrscr();
		return;
	}
	char ip2[target[1].size()+1];
	strcpy(ip2, target[1].c_str());
	char host2[target[0].size()+1];
	strcpy(host2, target[0].c_str());
	udpclient(6000,ip2); //needs rec port
	clrscr();
}
