#include <string.h>
#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <vector>
using namespace std;

//lists files for a given dir
vector<string> dirlist(string path)
{
	vector<string> filesv;
	for (const auto & file : filesystem::directory_iterator(path))
	{
		filesv.push_back(file.path());
	}
	//for (auto ir = filesv.crbegin(); ir != filesv.crend(); ++ir)
	//{
	        //cout << *ir << " ";
	//}
    return filesv;
}

//prints a vector, for use with dirlist
void dirprint(vector<string> files, int magic)
{
	int cnt =0;
	for (auto ir = files.crbegin(); ir != files.crend(); ++ir)
		{
				string tmp = *ir;

				printf("%i %s\n",cnt++,tmp.substr(magic).c_str());
		}
}
