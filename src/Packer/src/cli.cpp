#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "cliopts.h"
#include "colors.h"
#include "dirlist.h"

static const char options[] = "\n\
\e[1;32mChoose an option: \e[0;17m \n\n\
l - List Available Payloads \n\
a - Add new payload \n\
s - Send payload \n\
x - Exit \n\n\
\e[1;33m>\e[0m";

int cli(vector<string> files, string pathpacked, string pathstaging) {
		string inp;
		files = dirlist(pathpacked);
		clrscr();
		printf(TITLE);
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
				cout << "\nExiting...\n";
				//Kill ssh listener here
				break;
			} else {
				clrscr();
				cout << RED("Option not recognized.\n");
			}
		}
		return 0;
}
