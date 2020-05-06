/*
 * cliopts.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: cbai
 */

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "dirlist.h"
#include "Helpers.h"
#include "sshwraper.h"
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


void listpayloads(vector<string> files) {
	clrscr();
	printf(GREEN("Current payloads:\n"));
	//2 or 11 is magic, trust me  //ToDo, whoever wrote this - please clarify.
	dirprint(files,11);
}


void addpayload(string pathpacked, string pathstaging) {
	string ans, inp;
	int compression_ret;
	unsigned int id;
	vector<string> stage = dirlist(pathstaging);
	clrscr();
	printf(GREEN("The following payloads are availible in the staging folder:\n"));
	//10 is magic, trust me  //ToDo, whoever wrote this - please clarify.
	dirprint(stage, 10);
	printf("\nEnter the number for file to be packed ('x' to back out):\n");

	//Take user input to select file to turn into a payload
	while (1) {
		printf(YELLOW(">"));
		cin >> inp;
		try {
			id = std::stoi(inp);
			if (id>stage.size()-1) {
				cout << RED("Invalid option, please try again.");
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
	//cout <<"File :\"" << stage[id] << "\" selected.";

	// COMPRESSION <><>
	string compression_outp= pathpacked+stage[id].substr(10)+".zips";
	string iput = stage[id];
	compression_ret = ziphelp(iput, compression_outp);

	if (compression_ret == 0) { //Compression EXIT_SUCCESS is 0
		printf("File zipped.\n");
	} else {
		printf(RED("FILE ZIP FAILED.\n"));
	}

	/*//ENCRYPTION <><>
	string encryption_outp= pathpacked+stage[id].substr(10)+".encr";
	encrypthelp(compression_outp, encryption_outp);
	printf("File encrypted.\n");

	while (1) {
		printf("Do you want to delete the source in staging (y/n)?");
		printf(YELLOW(">"));
		cin >> ans;
		if (ans=="Y" or ans == "y") {
			if( remove( iput.c_str() ) != 0 )
				perror( "Error deleting file" );
			  else
				printf(GREEN("File successfully deleted"));
			break;
		} else if (ans=="n" or ans == "N") {
			break;
		} else {
			cout << RED("Invalid Option\n");
		}
	}*/

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




	clrscr();
}

void sendpayload(string pathpacked) {
	//ToDo logic to select server here <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	vector<string> target;
	target = findnew("Known_hosts");
	string host = target[0];
	string ip = target[1];
	char ip2[ip.size()+1];
	strcpy(ip2, ip.c_str());
	char host2[host.size()+1];
	strcpy(host2, host.c_str());
	printf("\n%s==\n%s==\n",host2,ip2);
	//connect(ip2, host2); No Longer Using SSH!
	//shit here
	udpclient(6000,ip2);//needs rec port
	clrscr();
}
