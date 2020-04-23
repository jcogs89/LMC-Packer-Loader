
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <Ws2tcpip.h>
#include <tchar.h>
//#include <iostream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#define Port 6000

SOCKET Socket, Sub;
WSADATA Winsock;
sockaddr_in Addr;
sockaddr_in IncomingAddress;
int AddressLen = sizeof(IncomingAddress);

int main()
{
    //client
    WSAStartup(MAKEWORD(2, 2), &Winsock);    // Start Winsock

    if (LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)    // Check version
    {
        WSACleanup();
        return 0;
    }

    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    ZeroMemory(&Addr, sizeof(Addr));    // clear the struct
    Addr.sin_family = AF_INET;    // set the address family
    InetPton(AF_INET, _T("192.168.2.3"), &Addr.sin_addr.s_addr); //set ip address?
    Addr.sin_port = htons(6000);    // set the port

    if (connect(Socket, (sockaddr*)&Addr, sizeof(Addr)) < 0)
    {
        printf("Connection failed !\n");
        getchar();
        return 0;
    }

    printf("Connection successful !\n");
    printf("Sending file .. \n");

    FILE* File;
    char* Buffer;
    unsigned long Size;

    fopen_s(&File, "E://Git/test.txt", "rb");
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

    fread(Buffer, Size, 1, File);
    char cSize[MAX_PATH];
    sprintf_s(cSize, "%i", Size);
    printf("%s\n", Buffer);

    fclose(File);
    send(Socket, cSize, MAX_PATH, 0); // File size

    //int len = Size;
    //char *data = Buffer;

    send(Socket, Buffer, Size + 1, 0);
    /*int Offset = 0;
    while (Size > Offset)
    {
        int Amount = send(Socket, Buffer + Offset, Size - Offset, 0);

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


    free(Buffer);
    closesocket(Socket);
    WSACleanup();
    //getchar();
    return 0;
}

