#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "cli.h"
#include "cliopts.h"

static const char options[] = "\e[1;1H\e[2J\n\
\e[1;32mChoose an option: \e[0;17m \n\
l - List Available Payloads \n\
a - Add new payload \n\
s - Send payload \n\
x - Exit \n\
\e[1;32m>\e[0;17m";

int cli(vector<string> files, string pathpacked, string pathstaging) {
		string inp;
		while (1) {
			printf(options);
			cin >> inp;
			if (inp == "l") {
				listpayloads(files);
			} else if (inp == "a") {
				addpayload(pathpacked, pathstaging);
			} else if (inp == "s") {
				sendpayload(pathpacked);
			} else if (inp == "x") {
				//Kill ssh listener here
				cout << "\nExiting.\n";
				break;
			} else {
				cout << "Option not recognized.\n";
			}
		}
		return 0;
}
