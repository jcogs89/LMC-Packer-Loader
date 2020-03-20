#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <iostream>
//#include "testing.h"
#include "dirlist.h"
#include "cli.h"
using namespace std;

//requires c++ v17
//-std=c++17

static const char usage[] = "\
\nUsage: packman [OPTIONS]... TARGET [HOSTNAME:PORT] PAYLOAD [FILE]\n\
Sends a user defined PAYLOAD to TARGET. \n\
Example: packman 192.168.0.1:1337 ~/payload.exe \n\n\
	-h, --help\tPrints this Help Page\n\
	-s \tStarts in service mode\
	\n\n";



int main(int argc, char *argv[]) {
	int option;
	char tvalue[32];
	char fvalue[32];
	//ToDo needs to be inported from config file
	string pathpacked = "./Payloads/";
	string pathstaging = "./Staging/";
	try
	{
		vector<string> files = dirlist(pathpacked);
	}
	catch (...)
	{
		printf("Payloads dir not found");
		exit(0);
	}
	vector<string> files = dirlist(pathpacked);	// ToDo why is this here twice?

	//dirprint(files);

	if(argc<2){ //Check to see if any arguments passed
		printf(usage);
		//testing();
		//string path = "./";
		//cout << dirlist(path)<< endl;
		//vector<string> files = dirlist(path);
		return 0;
	}

	// put ':' at the starting of the string so compiler can distinguish between '?' and ':'
	while((option = getopt(argc, argv, ":ht:f:s")) != -1){ //get option from the getopt() method
		switch(option){
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
				cli(files, pathpacked, pathstaging);
				return 1;
				break;
			return 0;
		}
	}
	//printf("Target Flag: %s\n",tvalue);
	//printf("File Flag: %s\n", fvalue);
	for(; optind < argc; optind++){ //when some extra arguments are passed
		printf("\nGiven extra arguments: %s\n", argv[optind]);
		printf(usage);
		return 0;
	}
}
