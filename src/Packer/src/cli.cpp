#include <iostream>

using namespace std;

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
		cout << "Invalid Option! Please enter a number.\n"
		}
}