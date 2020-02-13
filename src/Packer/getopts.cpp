#include <stdio.h>
#include <unistd.h>
#include <string.h>


static const char usage[] = "\
\nUsage: packman [OPTIONS]... TARGET [HOSTNAME:PORT] PAYLOAD [FILE]\n\
Sends a user defined PAYLOAD to TARGET. \n\
Example: packman 192.168.0.1:1337 ~/payload.exe \n\n\
	-h, --help\tPrints this Help Page\
	\n\n";

int main(int argc, char *argv[]) {
	int option;
	char tvalue[32];
	char fvalue[32];
	
	if(argc<2){ //Check to see if any arguments passed
		printf(usage);
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
			case '?': //used for some unknown options
				printf("\nUnknown option: %c\n", optopt);
			case 'h':
			printf(usage);
			return 0;
		}
	}
	printf("Target Flag: %s\n",tvalue);
	printf("File Flag: %s\n", fvalue);
	for(; optind < argc; optind++){ //when some extra arguments are passed
		printf("\nGiven extra arguments: %s\n", argv[optind]);
		printf(usage);
		return 0;
	}
}