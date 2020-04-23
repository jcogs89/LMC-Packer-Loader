#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;

#define clrscr() printf("\e[1;1H\e[2J")

void listpayloads(vector<string> files);
void addpayload(string pathpacked, string pathstaging);
void sendpayload(string pathpacked);
