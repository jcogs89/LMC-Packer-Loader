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


int forking () {
	pid_t parent = getpid();
	pid_t pid = fork();

	printf("\nstart fork\n");

	if (pid > 0) { // only good case
	   // parent process
	   printf("main process");
	   return 1;

	} else if (pid == 0) {
		// child process
	    printf("child process");
	    incom("knownhosts", parent);  //main of incom.cpp
	    printf("child died");

	} else {
		// fork failed
		printf("\nfork() failed!\n");
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
	std::string pathpacked;
	std::string pathstaging;

	ConfigFile cfg("./packer.conf"); //ToDo Add default conf file and option to pass CLA of its path

	//Read Config for required fields
	if (cfg.keyExists("payloads_dir")) {
		pathpacked = cfg.getValueOfKey<std::string>("payloads_dir");
		//printf("Config worked: %s\n", pathpacked.c_str());
	} else {
		printf("No payloads directory specified in config.  Please update 'payloads_dir' in the config."); //ToDo or ask user to input now
		exit(0);
	}
	if (cfg.keyExists("staging_dir")) {
		pathstaging = cfg.getValueOfKey<std::string>("staging_dir");
		//printf("Config worked: %s\n", pathstaging.c_str());
	} else {
		printf("No staging directory specified in config.  Please update 'staging_dir' in the config."); //ToDo or ask user to input now
		exit(0);
	}

	try {
		files = dirlist(pathpacked);
	}
	catch (...) {
		printf("Payloads dir (%s) not found", pathpacked.c_str());
		exit(0);
	}

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
				forking(); //ToDo Carl - what does forking to? Descriptive naming...
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
