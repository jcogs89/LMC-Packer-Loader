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
using namespace std;


void listpayloads(vector<string> files) {
	printf("current payloads\n\n");
	//2 or 11 is magic, trust me  //ToDo, whoever wrote this - please clarify.
	dirprint(files,11);
}


void addpayload(string pathpacked, string pathstaging) {
	string ans, inp;
	int compression_ret;
	unsigned int id;
	vector<string> stage = dirlist(pathstaging);

	printf("The following payloads are availible in the staging folder:\n");
	//10 is magic, trust me  //ToDo, whoever wrote this - please clarify.
	dirprint(stage, 10);
	printf("\nEnter the number for file to be packed ('x' to back out):");

	//Take user input to select file to turn into a payload
	while (1) {
		printf("\n>> ");
		cin >> inp;
		try {
			id = std::stoi(inp);
			if (id>stage.size()-1) {
				cout << "Invalid option, please try again.";
				continue;
			} else {
				printf("\n%i\n",id);
				break;
			}
		} catch (...) {
			if ((inp != "x") and (inp != "X")) {
				printf("Unrecognized input, please try again.");
			} else {
				return;
			}
		}
	}
	cout <<"File :\"" << stage[id] << "\" selected.";

	// COMPRESSION <><>
	string compression_outp= pathpacked+stage[id].substr(10)+".zips";
	string iput = stage[id];
	compression_ret = ziphelp(iput, compression_outp);

	if (compression_ret == 0) { //Compression EXIT_SUCCESS is 0
		printf("File zipped.\n");
	} else {
		printf("FILE ZIP FAILED.\n");
	}

	//ENCRYPTION <><>
	string encryption_outp= pathpacked+stage[id].substr(10)+".zips";
	encrypthelp(compression_outp, encryption_outp);
	printf("File encrypted.\n");

	while (1) {
		printf("Do you want to delete the source in staging (y/n)?\n>> ");
		cin >> ans;
		if (ans=="Y" or ans == "y") {
			if( remove( iput.c_str() ) != 0 )
				perror( "Error deleting file" );
			  else
				puts( "File successfully deleted" );
			break;
		} else if (ans=="n" or ans == "N") {
			break;
		} else {
			cout << "Invalid Option\n";
		}
	}
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
	connect(ip2, host2);
}
