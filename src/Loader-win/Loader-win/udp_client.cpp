#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <string>

#include "decryption.h"
#include "decompression.h"
#include "MemLoadLibrary.h"
//#include <iostream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#define Port 6000

SOCKET Socket, Sub;
WSADATA Winsock;
sockaddr_in Addr;
sockaddr_in IncomingAddress;
int AddressLen = sizeof(IncomingAddress);

int udp_server_clinet()
{
    //server
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
        printf("Client conncted!\n");
        //printf("IP: %s:%d\n", ClientIP, ClientPort);
        printf("Receiving file .. \n");



        int Size;
        char* Filesize = new char[1024];

        if (recv(Sub, Filesize, 1024, 0)) // File size
        {
            Size = atoi((const char*)Filesize);
            printf("File size: %d\n", Size);
        }
        else {
            printf("Failed to determine filesize of recieved file.");
                return 0;
        }

        char* Buffer = new char[Size];

        //int len = Size;
        //char *data = Buffer;
        int Offset = 0;
        int n = 0;
        while ((n = ::recv(Sub, Buffer + Offset, Size - Offset - 1, 0)) > 0) {
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
        printf("%s\n", Buffer);
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
        decryption();

        //decompression
        //ToDo change decompression from file to buffer input
        char* inBuf = Buffer;
        char* opBuf = new char[sizeof(Buffer)];
        //uziphelp(inBuf, opBuf);

        //run in memory
        //ToDo run in memory

        return 0;

    }
}