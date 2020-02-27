#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <iostream>
#include "testing.h"
#include "dirlist.h"
#include "cliopts.h"
using namespace std;

//requires c++ v17
//-std=c++17

static const char usage[] = "\
\nUsage: packman [OPTIONS]... TARGET [HOSTNAME:PORT] PAYLOAD [FILE]\n\
Sends a user defined PAYLOAD to TARGET. \n\
Example: packman 192.168.0.1:1337 ~/payload.exe \n\n\
	-h, --help\tPrints this Help Page\
	\n\n";

static const char options[] = "\
\n \e[1;32mChoose an option:\e[0;17m \n\
1. List Available Payloads \n\
2. Do some other stuff \n\n\
\e[1;32m>\e[0;17m ";
	
int cli() {
		int id;
		cout << options;
		cin >> id;
		
		if(id==1){
			cout << "Option 1 Selected\n";
		}
		else if(id==2){
			cout << "Option 2 Selected\n";
		}
		else{
		cout << "Invalid Option! Please enter a number.\n";
		}
}

int main(int argc, char *argv[]) {
	int option;
	char tvalue[32];
	char fvalue[32];

	if(argc<2){ //Check to see if any arguments passed
		printf(usage);
		//testing();
		//string path = "./";
		//cout << dirlist(path)<< endl;
		//vector<string> files = dirlist(path);
		return 0;
	}

	// put ':' at the starting of the string so compiler can distinguish between '?' and ':'
	while((option = getopt(argc, argv, ":ht:f:")) != -1){ //get option from the getopt() method
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
	cli();
}
