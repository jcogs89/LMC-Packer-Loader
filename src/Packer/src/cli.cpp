#include <iostream>
#include <string>
#include <vector>
#include "cliopts.h"

using namespace std;

static const char options[] = "\n \e[1;32m\
Choose an option: \e[0;17m \n\
1. List Available Payloads \n\
2. Add new payload \n\
3. Send payload \n\
0. Exit \n\
\e[1;32m>\e[0;17m ";
	

int cli(vector<string> files, string pathpacked, string pathstaging) {
		int id;

		while (1) {
			cout << options;
			cin >> id;
			if(id==1) {
				listpayloads(files);
			} else if(id==2) {
				addpayload(pathpacked, pathstaging);
			} else if(id==3) {
				sendpayload(pathpacked);
			} else if(id==0) {
				cout << "\nexit\n";
				break;
			} else {
				cout << "Invalid Option! Please enter a number.\n";
			}
		}
		return 0;
}
