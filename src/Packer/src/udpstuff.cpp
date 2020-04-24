/*
 * udpstuff.cpp
 *
 *  Created on: Apr 24, 2020
 *      Author: build
 */

#include "udpstuff.h"
#include "dirlist.h"

//#include <WinSock2.h>
//#include <Windows.h>
#include <stdio.h>
//#include <Ws2tcpip.h>
//#include <tchar.h>
//#include <iostream>
// server code for UDP socket programming
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "Helpers.h"
#include <iostream>
#include <fstream>
#include <filesystem>


#define IP_PROTOCOL 0

#define NET_BUF_SIZE 32
//#define cipherKey 'S'
#define sendrecvflag 0
#define nofile "File Not Found!"
using namespace std;
namespace fs = std::filesystem;


//SOCKET Socket, Sub;
//WSADATA Winsock;
sockaddr_in Addr;
sockaddr_in IncomingAddress;
int AddressLen = sizeof(IncomingAddress);

// function to clear buffer
void clearBuf(char* b)
{
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

// function sending file
int sendFile(FILE* fp, char* buf, int s)
{
    int i, len;
    if (fp == NULL) {
        strcpy(buf, nofile);
        len = strlen(nofile);
        buf[len] = EOF;
        return 1;
    }

    char ch, ch2;
    for (i = 0; i < s; i++) {
        ch = fgetc(fp);
        ch2 = ch;
        buf[i] = ch2;
        if (ch == EOF)
            return 1;
    }
    return 0;
}



int udpclient( int PORT_NO, char *IP_ADDRESS)
{
	//int sockfd, nBytes;
	int sockfd;
	struct sockaddr_in addr_con;
	unsigned int addrlen = sizeof(addr_con);
	addr_con.sin_family = AF_INET;
	addr_con.sin_port = htons(PORT_NO);
	addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	//char net_buf[NET_BUF_SIZE];
	FILE* fp;



  vector<string> stage = dirlist("./Payloads/");
  	printf("\n");
  	unsigned int id;
  	while (1)
  	{
  		//10 is magic, trust me
  		dirprint(stage, 10);
  		printf("\nEnter number for file to be packed\n>> ");
  		id = intinput();
  		if (id>stage.size()-1)
  		{
  			cout << "Invalid Option\n\n";
  			continue;
  		}
  		else
  		{
  			//cout << stage[id];
  			printf("\n%i\n",id);
  			break;
  		}
  	}
  	cout <<"File :\"" << stage[id] << "\" selected";
  	//jenk
  	//magic again
  	string outp= "./Payloads/"+stage[id].substr(10)+".zips";
  	string tmp = stage[id].substr(10);
  	//const char *fname= tmp.c_str();
  	const char *fname2= stage[id].c_str();
  	string iput = stage[id];

  	// socket()
	sockfd = socket(AF_INET, SOCK_STREAM , IP_PROTOCOL);

	if (sockfd < 0)
		printf("\nfile descriptor not received!!\n");
	else
		printf("\nfile descriptor %d received\n", sockfd);

	//while (1) {
	//printf("\nPlease enter file name to receive:\n");
	//scanf("%s", net_buf);
	//sendto(sockfd, net_buf, NET_BUF_SIZE,
	//	   sendrecvflag, (struct sockaddr*)&addr_con,
	//	   addrlen);

	//test connect
	connect(sockfd, (struct sockaddr*)&addr_con,  addrlen);

	printf("\n---------Data Received---------\n");

	//get fiel size
	fs::path p{fname2};
	p= fs::canonical(p);
	cout << "The size of " << p.u8string() << " is " << fs::file_size(p) << " bytes.\n";

	fp = fopen(fname2, "r");
	printf("\nFile Name Received: %s\n", fname2);
	if (fp == NULL)
	{
		printf("\nFile open failed!\n");
	}
	else
	{
		printf("\nFile Successfully opened!\n");
	}
	cin.get();
	//try shit here
	printf("fuck\n");
	char cSize[NET_BUF_SIZE];
	sprintf(cSize, "%i", fs::file_size(p));
	//sendto(sockfd, cSize, NET_BUF_SIZE, sendrecvflag, (struct sockaddr*)&addr_con, addrlen);
	send(sockfd, cSize, NET_BUF_SIZE, sendrecvflag);
	printf("fuck2\n");

	char* Buffer;
	Buffer = new char[fs::file_size(p)];
	fread(Buffer, fs::file_size(p), 1, fp);
	printf("%s\n", Buffer);

	send(sockfd, Buffer, fs::file_size(p), sendrecvflag);
	//while (1) {

		// process
		//if (sendFile(fp, net_buf, NET_BUF_SIZE))
		//{
		//	send(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag);
		//	break;
		//}
		//printf("%s",net_buf);
		// send
		//send(sockfd, net_buf, NET_BUF_SIZE, sendrecvflag);
		//clearBuf(net_buf);
	//}
	if (fp != NULL)
		fclose(fp);

	printf("\n-------------------------------\n");
	//}


	printf("\ndone\n");
	cin.get();
    return 0;
}
