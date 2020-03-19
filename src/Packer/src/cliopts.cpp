/*
 * cliopts.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: cbai
 */
#include <iostream>
#include<stdio.h>
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
	char ans;
	char y = 'y';
	char yc = 'Y';
	char n = 'n';
	while (1)
	{
		printf("Please put file in Staging folder\nIs the file in the staging folder (y/n)?\n>> ");
		cin >> ans;
		if (ans==y)
		{
			break;
		}
		else if (ans==yc)
		{
			break;
		}
		else if (ans==n)
		{
			continue;
		}
		else
		{
			cout << "Invalid Option\n";
		}

	}
	vector<string> stage = dirlist(pathstaging);

	printf("\n");
	unsigned int id;
	while (1)
	{
		//10 is magic, trust me
		dirprint(stage, 10);
		printf("\nEnter number for file to be packed\n>> ");
		id = intinput();
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
	//jenk
	printf("do somthing here\n");
	//magic again
	string outp= "./Payloads/"+stage[id].substr(10)+".zips";
	string iput = stage[id];
	ziphelp(iput, outp);
	//ret = uziphelp(outp, outp+".txt");

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
	//encryption goes here <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}


void sendpayload(string pathpacked)
{
	printf("\nsend a payload\n");
	connect();
}
