// Loader-win.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
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

int main() {
	// Temporary... file will eventually be pulled in from some .NET framework with networking while running as a service to recieve packed data.
	ConfigFile cfg("loader.conf");
	std::string port_num = get_config_item(cfg, "port");
	printf("The port is: %s\n", port_num.c_str());

	string inp;
	while (1) {
		cin >> inp;
		if (inp == "x") {
			break;
		}
		else {
			printf("%s", inp.c_str());
		}
	}

	return 0;
}

