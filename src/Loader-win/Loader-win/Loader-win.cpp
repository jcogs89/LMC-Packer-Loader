// Loader-win.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <stdio.h>
//#include "ssh_server.h"
#include "config_parser.h"
#include "decompression.h"
#include "decryption.h"

using namespace std;

string get_config_item(ConfigFile cfg, string item_name) {
	std::string item;
	if (cfg.keyExists(item_name)) {
		item = cfg.getValueOfKey<std::string>(item_name);
		//printf("Config worked: %s\n", item.c_str());
	}
	else {
		printf("No '%s' specified in packer.conf.  Please update the config and rerun the program.", item_name.c_str()); //ToDo or ask user to input now
		exit(0);
	}
	return item;
}

int main() {
	//Tlistnr()
	// Temporary... file will eventually be pulled in from some .NET framework with networking while running as a service to recieve packed data.
	ConfigFile cfg("loader.conf");
	std::string port_num = get_config_item(cfg, "port");
	std::string ssh_host_dsa_key = get_config_item(cfg, "ssh_host_dsa_key");
	std::string ssh_host_rsa_key = get_config_item(cfg, "ssh_host_rsa_key");
	printf("The port is: %s\n", port_num.c_str());

	// Removed ssh server for tcp listner
	//ssh_server(8833, ssh_host_dsa_key, ssh_host_rsa_key);

	//call tcp listener here

	//decryption
	decryption();

	//decompression
	string compresion_iput = "C:\\Users\\mitch\\Desktop\\Blush3ll\\src\\Loader-win\\Loader-win\\PAYLOADS\\poetry.zips";
	string compression_outp = "poetry.txt";
	uziphelp(compresion_iput, compression_outp);


	string inp;
	while (1) {
		cin >> inp;
		if (inp == "x") {
			break;
		}
		else {
			printf("\n>>");
		}
	}
	return 0;
}