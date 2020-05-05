#include <iostream>
#include <string>
#include <vector>

#include "cliopts.h"
#include "colors.h"

static const char options[] = "\n\
\e[1;32mChoose an option: \e[0;17m \n\n\
l - List Available Payloads \n\
a - Add new payload \n\
s - Send payload \n\
x - Exit \n\n\
\e[1;33m>\e[0m";

int cli(std::vector < std::string > files, std::string pathpacked, std::string pathstaging) {
		std::string inp;
		clrscr();
		std::cout << TITLE;
		while (1) {
			std::cout << options;
			std::cin >> inp;
			clrscr();
			if (inp == "l") {listpayloads(files);}
			else if (inp == "a") {addpayload(pathpacked, pathstaging);}
			else if (inp == "s") {sendpayload(pathpacked);}
			else if (inp == "x") {std::cout << "\nExiting...\n";break;}
			else {std::cout << RED("Option not recognized.\n");}
		}
		return 0;
}
