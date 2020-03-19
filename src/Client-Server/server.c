//Server side C/C++ program to demonstrate Socket programming for linux.
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

int server() 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
       
    //Creating socket file descriptor (IPV4, TCP, IP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    //This is completely optional, but it helps in reuse of address and port. Prevents error such as: “address already in use”.
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }
	
	//Set socket address structure values
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; //Setting address of local host to bind to
    address.sin_port = htons( PORT ); //Port set by #define
       
    //Bind the socket to the address and port number specified in address above
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
	
	//It puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection. 
	//The backlog, defines the maximum length to which the queue of pending connections for sockfd may grow. 
	//If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED.
    if (listen(server_fd, 3) < 0)
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
	
	//Extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket.
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
	
	//Connection is established between client and server, and they are ready to transfer data.
	
	//Read data from client into a buffer and print that data.
    valread = read( new_socket , buffer, 1024); 
    printf("%s\n",buffer ); 
	
	//Send data to the client from a buffer.
    send(new_socket , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n");
    return 0; 
} 
