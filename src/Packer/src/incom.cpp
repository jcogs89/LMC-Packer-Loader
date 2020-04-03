/*
 * incom.cpp
 *
 *  Created on: Apr 1, 2020
 *      Author: cbai
 */
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include "sshwraper.h"
#include "sshserver.h"
#include "dirlist.h"
#include <vector>
using namespace std;

string findnew (string knownhostsfolder)
{
	int magic = 12;
	vector<string> hostsf= dirlist("Known_hosts");
	//ofstream hosts;
	//hosts.open (knownhostsfile, ios::trunc);

	long unsigned int cnt =0;
	printf("The following systems are known:\n");
	//10 is magic, trust me  //ToDo, whoever wrote this - please clarify.
	for (cnt=0; cnt !=hostsf.size();cnt++)
	{
		//string tmp = *ir;
		printf("%li %s\n",cnt,hostsf[cnt].substr(magic).c_str());
		//printf("%i %s\n",cnt,tmp.substr(magic).c_str());

	}
	printf("\nEnter the number for system to be packed ('x' to back out):");

	//hosts << "Writing this to a file.\n";
	//hosts.close();
	string inp;
	unsigned int id;
	while (1) {
			printf("\n>> ");
			cin >> inp;
			try {
				id = std::stoi(inp);
				if (id>hostsf.size()-1) {
					cout << "Invalid option, please try again.";
					continue;
				} else {
					//printf("\n%i\n",id);
					cout <<"File :\"" << hostsf[id].substr(magic) << "\" selected";
					break;
				}
			} catch (...) {
				if ((inp != "x") and (inp != "X")) {
					printf("Unrecognized input, please try again.");
				} else {
					exit(0);
				}
			}
		}
		return hostsf[id].substr(magic).c_str();
}

void incom(string knownhostsfile)
{
	while(1)
	{
		//fs << " more lorem ipsum\n";
		sshserver(8833,"./Key/ssh_host_dsa_key", "./Key/ssh_host_rsa_key");
		//a new connectoin has been added

	}
}
