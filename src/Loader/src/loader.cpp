#include <iostream>
#include <fstream>
using namespace std;


int main() {
	// Temporary... file will eventually be pulled in from some .NET framework with networking while running as a service to recieve packed data.
		ifstream infile;
		char data[100]; 
		
		infile.open("hello_world.txt", ios::in);
		
		cout << "Reading from the file" << endl;
		infile >> data;
		
		// write data to screen.
		cout << data << endl;
		
		// close opened input file.
		infile.close();
	
	return 0;
}