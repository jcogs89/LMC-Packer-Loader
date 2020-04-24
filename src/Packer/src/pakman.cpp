//Requires C++ v17

#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string.h>

#include "cli.h"
#include "colors.h"
#include "dirlist.h"
#include "config_parser.h"
#include "incom.h"

static const char usage[] = "\n\
Usage: packman [OPTIONS]... TARGET [HOSTNAME:PORT] PAYLOAD [FILE] \n\
Sends a user defined PAYLOAD to TARGET. \n\
Example: packman 192.168.0.1:1337 ~/payload.exe \n\n\
	-h, --help\tPrints this Help Page\n\
	-s \t\tStarts in Service Mode\
	\n\n";

string get_config_item(ConfigFile cfg, string item_name) {
	std::string item;
	if (cfg.keyExists(item_name)) {
		item = cfg.getValueOfKey<std::string>(item_name);
		//printf("Config worked: %s\n", item.c_str());
	} else {
		printf("No '%s' specified in packer.conf.  Please update the config and rerun the program.", item_name.c_str()); //ToDo or ask user to input now
		exit(1);
	}
	return item;
}

int fork_ssh_listener_process (std::string ssh_host_dsa_key, std::string ssh_host_rsa_key) {
	pid_t parent = getpid();
	pid_t pid = fork();

	//printf("\nstart fork\n");

	if (pid > 0) { //main process //ToDo Carl - what does this do?
	   //printf("main process");
	   return 0;

	} else if (pid == 0) { // child process
		//printf("Child process created for ssh listener.\n");
	    incom("knownhosts", parent, ssh_host_dsa_key, ssh_host_rsa_key);  //main of incom.cpp
	    //printf("child ssh listener died.");

	} else { // fork failed
		//printf("\nforking ssh_listener_process failed!\n");
	}
	exit(1);
}

int main(int argc, char *argv[]) {
	int option;
	char tvalue[32];
	char fvalue[32];
	bool smode = false;
	vector<string> files;

	ConfigFile cfg("./packer.conf"); //ToDo Add default conf file and option to pass CLA of its path
	std::string pathpacked = get_config_item(cfg, "payloads_dir");
	std::string pathstaging = get_config_item(cfg, "staging_dir");
	std::string ssh_host_dsa_key = get_config_item(cfg, "ssh_host_dsa_key");
	std::string ssh_host_rsa_key = get_config_item(cfg, "ssh_host_rsa_key");

	files = dirlist(pathpacked);

	if(argc<2){
		printf(RED("No arguments given"));
		printf(usage);
		return 1;
	}

	//Put ':' at the starting of the string so compiler can distinguish between '?' and ':'
	while((option = getopt(argc, argv, ":ht:f:s")) != -1) { //Get option from the getopt() method
		switch(option) { //Check each give option against allowed values
			case 't':
				memcpy(tvalue, optarg, strlen(optarg)+1);
				printf(BLUE("Target Flag: %s\n"),tvalue);
				break;
			case 'f':
				memcpy(fvalue, optarg, strlen(optarg)+1);
				printf(BLUE("File Flag: %s\n"), fvalue);
				break;
			case ':':
				printf(RED("Option %c needs a value"), optopt);
				printf(usage);
				return 1;
			case '?':
				printf(RED("Unknown option: %c"), optopt);
				printf(usage);
				return 1;
			case 'h':
				printf(usage);
				return 1;
			case 's':
				smode = true;
				break;
		}
	}

	for(; optind < argc; optind++) { //Check if extra arguments passed
		printf(RED("Given extra arguments: %s"), argv[optind]);
		printf(usage);
		return 1;
	}

	if(smode) {
		fork_ssh_listener_process(ssh_host_dsa_key, ssh_host_rsa_key);
		cli(files, pathpacked, pathstaging); //Enter Main Execution
		return 0;
	}
	printf(RED("This should never happen. Please run in service mode with -s"));
	return 1;
}
