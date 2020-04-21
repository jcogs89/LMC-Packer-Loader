/*
 * incom.h
 *
 *  Created on: Apr 1, 2020
 *      Author: cbai
 */
#include <string.h>
void incom(string knownhostsfile, pid_t parent, std::string ssh_host_dsa_key, std::string ssh_host_rsa_key);
vector<string> findnew (string knownhostsfolder);
