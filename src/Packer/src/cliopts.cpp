/*
 * cliopts.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: cbai
 */
#include <iostream>
#include<stdio.h>
#include <string>
#include <vector>
#include "dirlist.h"
using namespace std;

void listpayloads(vector<string> files)
{
	printf("current payloads\n\n");
	//2 or 11 is magic, trust me
	dirprint(files,11);
}

void addpayload(string pathpacked, string pathstaging)
{
	char ans;
	char y = 'y';
	char yc = 'Y';
	char n = 'n';
	while (1)
	{
		printf("Please put file in Staging folder\nIs the file in the staging folder (y/n)?");
		cin >> ans;
		if (ans==y)
		{
			break;
		}
		else if (ans==yc)
		{
			break;
		}
		else if (ans==n)
		{
			continue;
		}
		else
		{
			cout << "Invalid Option\n";
		}

	}
	vector<string> stage = dirlist(pathstaging);
	//10 is magic, trust me
	printf("\n");
	dirprint(stage, 10);

}
