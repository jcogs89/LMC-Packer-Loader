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
#include "incom.h"
#include <vector>
using namespace std;

vector<string> findnew (string knownhostsfolder)
{
	int magic = 12; // "Known_hosts" is 12 bytes ;)
	vector<string> hostsf= dirlist("Known_hosts");
	//ofstream hosts;
	//hosts.open (knownhostsfile, ios::trunc);
	long unsigned int cnt =0;
	printf("The following systems are known:\n");

	for (cnt=0; cnt !=hostsf.size();cnt++){
		//string tmp = *ir;

		string name = hostsf[cnt];
		string hostname = name.substr(magic,name.find("@")-magic);
		string ipaddress = name.substr(name.find("@")+1);
		//cout << hostname << " at " << ipaddress << "\n";

		printf("%li %s@%s\n",cnt,hostname.c_str(),ipaddress.c_str());
	}

	cout << "\nEnter the number for system to be packed ('x' to back out):";

	//hosts << "Writing this to a file.\n";
	//hosts.close();

	string selec;
	string host;
	string addr;
	vector<string> target (2);
	unsigned int id;
	string inp;
	while (1) {
			cout << "\n>> ";
			cin >> inp;
			try {
				id = stoi(inp);
				if (id>hostsf.size()-1) {
					cout << "Invalid option, please try again.";
					continue;
				} else {
					//printf("\n%i\n",id);
					selec = hostsf[id];
					host = selec.substr(magic,selec.find("@")-magic);
					addr = selec.substr(selec.find("@")+1);
					target = {host, addr};
					cout <<"File :\"" << host << "@" << addr << "\" selected";
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
		return target;
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
