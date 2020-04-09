/*
 * incom.cpp
 *
 *  Created on: Apr 1, 2020
 *      Author: cbai
 */
#include <stdio.h>
#include <signal.h>
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
	printf("\e[1;32mThe following systems are known:\e[0;17m\n");
	//10 is magic, trust me  //ToDo, whoever wrote this - please clarify.
	for (cnt=0; cnt !=hostsf.size();cnt++)
	{
		//string tmp = *ir;
		printf("%li %s\n",cnt,hostsf[cnt].substr(magic).c_str());
		//printf("%i %s\n",cnt,tmp.substr(magic).c_str());

	}
	printf("\nChoose a target host ('x' to back out):"); //ToDo allow for multiple targets

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
				if ((inp == "x") or (inp == "X")) {
					return "0";
				} else {
					printf("Unrecognized input, please try again.");
				}
			}
		}
		return hostsf[id].substr(magic).c_str();
}

void incom(string knownhostsfile, pid_t parent, std::string ssh_host_dsa_key, std::string ssh_host_rsa_key) {
	while(1) {
		sshserver(8833, ssh_host_dsa_key, ssh_host_rsa_key); //ToDo add to config
		//ToDo Carl - "comment this out later"
		if (0 == kill(parent, 0)) {
		    // Process exists.
		} else {
			exit(0);
		}
		//a new connection has been added
	}
}
