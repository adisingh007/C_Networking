/******************************************************************************************
 * This is a simple client program implemented in C programming language.                 *
 * This program connects to a server, sends a message "Hi, Server!" to it,                *
 * receives a response message from the same server, and finally prints it.               *
 *                                                                                        *
 * Author - Aditya Singh                                                                  *
 ******************************************************************************************/

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>

#define MAX_BUFF 256 // Max size of incoming message

typedef struct sockaddr SockAddr; 
typedef struct sockaddr_in SockAddrIn;
typedef struct hostent HostEnt;


/*
 * This function creates the server address that is to be connected to.
 *
 * Algorithm:
 *     1 - Assign family of the server address to AF_INET.
 *     2 - Assign the port number of the server address using htons() function.
 *     3 - Copy the server's IP address to the socket address section.
 *     4 - return the server address.
 */
SockAddrIn getServAddr(int port, char *host) {
	
	SockAddrIn servAddr;
	HostEnt *server = gethostbyname(host);
	
	servAddr.sin_family  = AF_INET;
	servAddr.sin_port    = htons(port); 
	bcopy((char *)server->h_addr, 
		  (char *)&servAddr.sin_addr.s_addr,
		  server->h_length);
		  
	return servAddr;	  
}


/*
 * This function creates the connection to the server.
 *
 * Algorithm:
 *     1 - Create a socket.
 *     2 - Get the server address from the getServAddr() function.
 *     3 - Connect to the server.
 *     4 - return the socket ID.
 */
int getSocketFd(int port, char *host) {
	
	int sockFd = socket(AF_INET, SOCK_STREAM, 0);
	
	SockAddrIn servAddr = getServAddr(port, host);
		  
	connect(sockFd, (SockAddr *)&servAddr, sizeof(servAddr));
	
	return sockFd;
}

/*
 * Steps of working:                                                                       
 *     1 - Port number and server IP address is accepted via command line args                                  
 *     2 - Server Address is created                                                     
 *     3 - Connection is made to that server                                     
 *     4 - A message is sent to the server
 *     5 - A response message is read received from the server                                                   
 *     6 - The receiver message is printed
 *     7 - Client socket is closed                                       
 *     8 - Program terminates                                                             
 */
int main(int argc, char *argv[]) {

	char *host = argv[1];
	int portNo  = atoi(argv[2]);
	int sockFd = getSocketFd(portNo, host);
	char buffer[MAX_BUFF];
	
	char msg[] = "Hi, Server!";
	
	int n = write(sockFd, msg, strlen(msg));
	    n = read(sockFd, buffer, MAX_BUFF - 1);

	printf("%s\r\n", buffer);


	close(sockFd);
	return 0;
}
