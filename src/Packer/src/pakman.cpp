//Requires C++ v17

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "cli.h"
#include "colors.h"
#include "config_parser.h"
#include "dirlist.h"

static const char usage[] = "\n\
Usage: pakman [OPTIONS]... TARGET [HOSTNAME:PORT] PAYLOAD [FILE] \n\
Sends a user defined PAYLOAD to TARGET. \n\
Example: pakman 192.168.0.1:1337 ~/payload.exe \n\n\
	-h, --help\tPrints this Help Page\n\
	-s \t\tStarts in Service Mode\
	\n\n";

std::string get_config_item(ConfigFile cfg, std::string item_name) {
	std::string item;
	if (cfg.keyExists(item_name)) {
		item = cfg.getValueOfKey<std::string>(item_name);
		Log("Config worked: " << item << "\n");
	} else {
		std::cout << "No " << item_name << " specified in packer.conf.  Please update the config and rerun the program.";
		exit(1);
	}
	return item;
}

int main(int argc, char *argv[]) {
	int option;
	char tvalue[32];
	char fvalue[32];
	bool smode = false;

	std::vector<std::string> files;

	ConfigFile cfg("./packer.conf"); //ToDo Add default conf file and option to pass CLA of its path
	std::string pathpacked = get_config_item(cfg, "payloads_dir");
	std::string pathstaging = get_config_item(cfg, "staging_dir");
	std::string ssh_host_dsa_key = get_config_item(cfg, "ssh_host_dsa_key");
	std::string ssh_host_rsa_key = get_config_item(cfg, "ssh_host_rsa_key");

	files = dirlist(pathpacked);

	if(argc < 2) {
		std::cout << RED("No arguments given");
		std::cout << usage;
		return 1;
	}

	//Put ':' at the starting of the string so compiler can distinguish between '?' and ':'
	while((option = getopt(argc, argv, ":ht:f:s")) != -1) { //Get option from the getopt() method
		switch(option) { //Check each given option against allowed values
			case 't':
				memcpy(tvalue, optarg, strlen(optarg)+1);
				Log("Target Flag: " << tvalue << "\n");
				break;
			case 'f':
				memcpy(fvalue, optarg, strlen(optarg)+1);
				Log("File Flag: " << fvalue << "\n");
				break;
			case ':': std::cout << RED("Option " << optopt << " needs a value") << usage; return 1;
			case '?': std::cout << RED("Unknown option: " << optopt << "") << usage; return 1;
			case 'h': std::cout << usage; return 1;
			case 's': smode = true; break;
		}
	}

	for(; optind < argc; optind++) { //Check if extra arguments passed
		std::cout << RED("Given extra arguments: " << argv[optind] << "") << usage;
		return 1;
	}

	if(smode) {
		cli(files, pathpacked, pathstaging); //Enter Main Execution
		clrscr();
		std::cout << "Thanks for using...\n" << TITLE;
		return 0;
	}

	std::cout << "This should never happen. Please run in service mode with -s" << TITLE;
	return 1;

}
