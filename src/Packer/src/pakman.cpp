#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <iostream>
//#include "testing.h"
#include "dirlist.h"
#include "cli.h"
#include "incom.h"
#include "parser2.h"

using namespace std;

//requires c++ v17
//-std=c++17


static const char usage[] = "\n\
Usage: packman [OPTIONS]... TARGET [HOSTNAME:PORT] PAYLOAD [FILE]\n\
Sends a user defined PAYLOAD to TARGET. \n\
Example: packman 192.168.0.1:1337 ~/payload.exe \n\n\
	-h, --help\tPrints this Help Page\n\
	-s \tStarts in service mode\
	\n\n";


int forking ()
{
	printf("\nstart fork\n");
	pid_t pid = fork();
	if (pid == 0)
	{
		// child process
	    printf("child process");
	    incom("knownhosts");
	    printf("child died");
	    exit(0);
	    return 0;
	}
	else if (pid > 0)
	{
	// parent process
	    printf("main process");
		return 1;
	}
	else
	{
	// fork failed
	printf("\nfork() failed!\n");
	exit(0);
	return(-69);
	}
	exit(0);
	return(-69);
}


int main(int argc, char *argv[]) {
	int option;
	char tvalue[32];
	char fvalue[32];
	vector<string> files;
	string pathpacked = "./Payloads/";	// ToDo add to config
	string pathstaging = "./Staging/";	// ToDo add to config
	try {
		files = dirlist(pathpacked);
	}
	catch (...) {
		printf("Payloads dir not found");
		exit(0);
	}

	ConfigFile cfg("./example.conf");

	bool exists = cfg.keyExists("port");
	std::cout << "port: " << std::boolalpha << exists << "\n";

	std::string portValue = cfg.getValueOfKey<std::string>("port");
	std::cout << "value of key port: " << portValue << "\n";


	//dirprint(files);

	//program requires one argument
	//ToDo do we want to feed more arguments?  No arguments (if using config)?
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
			case '?': //used for some unknown options
				printf("\nUnknown option: %c\n", optopt);
				break;
			case 'h':
				printf(usage);
				//testing();
				break;
			case 's':
				printf("Starting\n");
				//forking(); //ToDo this was broken, who last pushed??
				cli(files, pathpacked, pathstaging);
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
