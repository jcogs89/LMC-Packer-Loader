#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "colors.h"

//Returns a vector of file stings for a given dir
std::vector<std::string> dirlist(std::string path){
	std::vector<std::string> filesv;
	try {
		for (const auto & file : std::filesystem::directory_iterator(path)) {
			filesv.push_back(file.path());
		}
		//for (auto ir = filesv.crbegin(); ir != filesv.crend(); ++ir)
		//{
				//cout << *ir << " ";
		//}
		return filesv;

	} catch (const std::filesystem::filesystem_error &) {
		std::cout << RED("Directory " << path << " not found.");
		exit(1);
	}
}

//Prints a std::vector, for use with dirlist
void dirprint(std::vector<std::string> files, int magic){
	long unsigned int cnt =0;
	//for (auto ir = files.crbegin(); ir != files.crend(); ++ir)
	for (cnt=0; cnt != files.size(); cnt++){
		//std::string tmp = *ir;
		std::cout << cnt << " - " << files[cnt].substr(magic) << "\n";
		//printf("%i %s\n",cnt,tmp.substr(magic).c_str());
	}
}
