/*
 * incom.h
 *
 *  Created on: Apr 1, 2020
 *      Author: cbai
 */

void incom(std::string knownhostsfile, pid_t parent, std::string ssh_host_dsa_key, std::string ssh_host_rsa_key);
std::vector<std::string> findnew (std::string knownhostsfolder);
