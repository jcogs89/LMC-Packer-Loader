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
using namespace std;

void listpayloads(vector<string> files)
{
	printf("current payloads\n\n");
	//2 or 11 is magic, trust me
	dirprint(files,11);
}

void addpayload(string pathpacked, string pathstaging)
{
	char y = 'y';
	char yc = 'Y';
	char n = 'n';
	char ans;

	printf("The following payloads are availible in the staging folder:");
	vector<string> stage = dirlist(pathstaging);
	printf("\n");


	//ToDo add way to back out gracefully
	unsigned int id;
	while (1)
	{
		//10 is magic, trust me
		dirprint(stage, 10);
		printf("\nEnter number for file to be packed.\n>> ");
		id = intinput();

		printf("%d", id);
		if (id>stage.size()-1)
		{
			cout << "Invalid Option\n\n";
			continue;
		}
			else
		{
			//cout << stage[id];
			printf("\n%i\n",id);
			break;
		}
	}
	cout <<"File :\"" << stage[id] << "\" selected";


	// COMPRESSION <><>
	string compression_outp= "./Payloads/"+stage[id].substr(10)+".zips";
	string iput = stage[id];
	ziphelp(iput, compression_outp);
	//ret = uziphelp(outp, outp+".txt");


	//ENCRYPTION <><>
	string encryption_outp= "./Payloads/"+stage[id].substr(10)+".encr";
	encrypthelp(compression_outp, encryption_outp);


	while (1)
		{
			printf("Do you want to delete the source in staging (y/n)?\n>> ");
			cin >> ans;
			if (ans==y or ans == yc)
			{
				if( remove( iput.c_str() ) != 0 )
				    perror( "Error deleting file" );
				  else
				    puts( "File successfully deleted" );
				break;
			}
			else if (ans==n)
			{
				break;
			}
			else
			{
				cout << "Invalid Option\n";
			}

		}
}


void sendpayload(string pathpacked)
{
	printf("\nsend a payload\n");
	//logic to select server here <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	connect("127.0.0.1");
}
