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

string findnew (string knownhostsfile, string knownhostsfolder)
{
	int magic = 10;
	vector<string> hostsf= dirlist("Known_hosts");
	//ofstream hosts;
	//hosts.open (knownhostsfile, ios::trunc);

	long unsigned int cnt =0;
	for (cnt=0; cnt !=hostsf.size();cnt++)
	{
		//string tmp = *ir;

		printf("%li %s\n",cnt,hostsf[cnt].substr(magic).c_str());

		//printf("%i %s\n",cnt,tmp.substr(magic).c_str());

	}

	//hosts << "Writing this to a file.\n";
	//hosts.close();
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
