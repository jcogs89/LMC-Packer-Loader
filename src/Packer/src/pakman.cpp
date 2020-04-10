//requires c++ v17  //ToDo reconfirm this is true
//-std=c++17

#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <iostream>

#include "dirlist.h"
#include "cli.h"
#include "config_parser.h"
#include "incom.h"
//#include "testing.h"

using namespace std;

static const char usage[] = "\n\
Usage: packman [OPTIONS]... TARGET [HOSTNAME:PORT] PAYLOAD [FILE] \n\
Sends a user defined PAYLOAD to TARGET. \n\
Example: packman 192.168.0.1:1337 ~/payload.exe \n\n\
	-h, --help\tPrints this Help Page\n\
	-s \tStarts in service mode\
	\n\n";

string get_config_item(ConfigFile cfg, string item_name) {
	std::string item;
	if (cfg.keyExists(item_name)) {
		item = cfg.getValueOfKey<std::string>(item_name);
		//printf("Config worked: %s\n", item.c_str());
	} else {
		printf("No '%s' specified in packer.conf.  Please update the config and rerun the program.", item_name.c_str()); //ToDo or ask user to input now
		exit(0);
	}
	return item;
}


int fork_ssh_listener_process (std::string ssh_host_dsa_key, std::string ssh_host_rsa_key) {
	pid_t parent = getpid();
	pid_t pid = fork();

	//printf("\nstart fork\n");

	if (pid > 0) { //main process //ToDo Carl - what does this do?
	   //printf("main process");
	   return 1;

	} else if (pid == 0) { // child process
		printf("Child process created for ssh listener.\n");
	    incom("knownhosts", parent, ssh_host_dsa_key, ssh_host_rsa_key);  //main of incom.cpp
	    printf("child ssh listener died.");

	} else { // fork failed
		printf("\nforking ssh_listener_process failed!\n");
	}
	exit(0);
	return(0);
}


int main(int argc, char *argv[])
{
	int option;
	char tvalue[32];
	char fvalue[32];
	vector<string> files;

	ConfigFile cfg("./packer.conf"); //ToDo Add default conf file and option to pass CLA of its path
	std::string pathpacked = get_config_item(cfg, "payloads_dir");
	std::string pathstaging = get_config_item(cfg, "staging_dir");
	std::string ssh_host_dsa_key = get_config_item(cfg, "ssh_host_dsa_key");
	std::string ssh_host_rsa_key = get_config_item(cfg, "ssh_host_rsa_key");

	files = dirlist(pathpacked);


	//READING FROM CONFIG FILE DEBUG
	//bool exists = cfg.keyExists("port");
	//std::cout << "port: " << std::boolalpha << exists << "\n";
	//std::string portValue = cfg.getValueOfKey<std::string>("port");
	//std::cout << "value of key port: " << portValue << "\n";

	//program requires one argument
	//ToDo do we want to feed more arguments?  Or feed no arguments? (if using config)
	if(argc<2) {
		printf(usage);
		return 0;
	}

	// put ':' at the starting of the string so compiler can distinguish between '?' and ':'
	while((option = getopt(argc, argv, ":ht:f:s")) != -1) { //get option from the getopt() method
		switch(option) {
			case 't':
				memcpy(tvalue, optarg, strlen(optarg)+1);
				printf("Target Flag: %s\n",tvalue);
				break;
			case 'f':
				memcpy(fvalue, optarg, strlen(optarg)+1);
				printf("File Flag: %s\n", fvalue);
				break;
			case ':':
				printf("\nOption %c needs a value\n", optopt);
				break;
			case '?':
				printf("\nUnknown option: %c\n", optopt);
				break;
			case 'h':
				printf(usage);
				break;
			case 's':
				fork_ssh_listener_process(ssh_host_dsa_key, ssh_host_rsa_key);
				printf("Hello.");
				cli(files, pathpacked, pathstaging); //main execution
				return 1;
				break;
			return 0;
		}
	}

	for (; optind < argc; optind++) { //when some extra arguments are passed
		printf("\nGiven extra arguments: %s\n", argv[optind]);
		printf(usage);
		return 0;
	}
}
