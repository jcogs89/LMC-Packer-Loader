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
#include "colors.h"
#include <vector>
using namespace std;

vector<string> findnew (string knownhostsfolder)
{
	int magic = 12; // "Known_hosts" is 12 bytes ;)
	vector<string> hostsf= dirlist("Known_hosts");
	//ofstream hosts;
	//hosts.open (knownhostsfile, ios::trunc);
	long unsigned int cnt =0;
	clrscr();
	printf(GREEN("The following systems are known:\n"));
	//10 is magic, trust me  //ToDo, whoever wrote this - please clarify.
	for (cnt=0; cnt !=hostsf.size();cnt++)
	{
		//string tmp = *ir;

		string name = hostsf[cnt];
		string hostname = name.substr(magic,name.find("@")-magic);
		string ipaddress = name.substr(name.find("@")+1);
		//cout << hostname << " at " << ipaddress << "\n";

		printf("%li %s@%s\n",cnt,hostname.c_str(),ipaddress.c_str());
	}
	printf(GREEN("\nChoose a target host ('x' to back out):")); //ToDo allow for multiple targets

	//hosts << "Writing this to a file.\n";
	//hosts.close();

	string selec;
	string host;
	string addr;
	vector<string> target (2);
	unsigned int id;
	string inp;
	while (1) {
			clrscr();
			cin >> inp;
			try {
				id = stoi(inp);
				if (id>hostsf.size()-1) {
					cout << RED("Invalid option, please try again.");
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
				if ((inp == "x") or (inp == "X")) {
					return target;
				} else {
					printf("Unrecognized input, please try again.");
				}
			}
		}
		return target;
}

void incom(string knownhostsfile, pid_t parent, std::string ssh_host_dsa_key, std::string ssh_host_rsa_key) {
	while(1) {
		if(-1==sshserver(8833, ssh_host_dsa_key, ssh_host_rsa_key)) //ToDo add to config
		{
			Log("\nfailed to establish listen service\ncheck port is open\n");
			exit(-1);
		}
		//ToDo Carl - "comment this out later"
		if (0 == kill(parent, 0)) {
		    // Process exists.
		} else {
			exit(0);
		}
		//a new connection has been added
	}
}
