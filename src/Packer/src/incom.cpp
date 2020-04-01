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
using namespace std;

void findnew (string knownhostsfile, string knownhostsfolder)
{
	dirlist("Known_hosts");
	ofstream hosts;
	hosts.open (knownhostsfile, ios::trunc);
	hosts << "Writing this to a file.\n";
	hosts.close();
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
