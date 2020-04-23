/*
 * cli.h
 *
 *  Created on: Feb 27, 2020
 *      Author: cbai
 */
#include <string>
#include <vector>

#define clrscr() printf("\e[1;1H\e[2J")

using namespace std;
int cli(vector<string> files,string pathpacked, string pathstaging);
