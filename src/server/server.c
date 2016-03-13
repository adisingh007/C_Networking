/******************************************************************************************
 * This is a simple server program implemented in C programming language.                 * 
 * This program accepts client connections, accepts a String message                      *  
 * from the currently connected client, and replies back with "Hello, Client!" message.   *
 *                                                                                        *
 * Author - Aditya Singh                                                                  *
 ******************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>

#define MAX_BUFF 256 // Max size of incoming message


typedef struct sockaddr    SockAddr;
typedef struct sockaddr_in SockAddrIn;
typedef        socklen_t   SockLen;


/*
 * This function creates a server address.
 * 
 * Algorithm:
 * 	   1 - Sets the family of server socket to AF_INET.
 *     2 - Sets the port number using htons(port) function from .
 *     3 - Sets the address of the server to any available ip address/addresses.
 *     4 - returns the server address.
 */
SockAddrIn getServAddr(int port) {

	SockAddrIn servAddr;
	
	servAddr.sin_family      = AF_INET;
	servAddr.sin_port        = htons(port);
	servAddr.sin_addr.s_addr = INADDR_ANY; 
	
	return servAddr;
}


/*
 * This function makes the server socket listen to a particular port number.
 *
 * Algorithm:
 *     1 - Receive the server address reference from getServAddr() function.
 *     2 - Create a socket.
 *     3 - Bind the server address to the socket.
 *     4 - Make the server socket listen on the port.
 *     5 - return the reference ID of the server socket
 */
int getSockFd(int port) {

	SockAddrIn servAddr = getServAddr(port);
	long servAddrLength = sizeof(servAddr);
	int sockFd = socket(AF_INET, SOCK_STREAM, 0);
	
	int bindFd = bind(sockFd, (SockAddr *)&servAddr, servAddrLength);
	listen(sockFd, 5);
	
	return sockFd;
}


/*
 * This function creates a client socket.
 *
 * Algorithm:
 *     1 - Create a client address variable.
 *     2 - Get the length of that variable(type = socklen_t).
 *     3 - Wait for the client to connect.
 *     4 - After a client is connected, return the client socket id.	
 */
int getClientFd(int sockFd) {

	SockAddrIn clientAddr;
	SockLen clientLength = sizeof(clientAddr);
	int clientFd = accept(sockFd, (SockAddr *)&clientAddr, &clientLength);
	
	return clientFd;
}

/*
 * Steps of working:                                                                       
 *     1 - Port number is accepted via command line args                                  
 *     2 - Server Socket is created                                                       
 *     3 - A single client is allowed to connect, followed by creation of a client socket 
 *     4 - Message is read from the client                                                
 *     5 - A message is sent to the client in response                                       
 *     6 - Client socket is closed  
 *     7 - Go back to step 3
 *
 * ---------------------------------
 *  If the program is terminated:                                                      
 *     8 - Server socket is closed                                                        
 *     9 - Program terminates                                                             
 */
int main(int argc, char *argv[]) {

	int portNo = atoi(argv[1]);
	
	int sockFd    = getSockFd(portNo);
	int clientFd;
	
	char buff[MAX_BUFF];
	char *headers;
	char *msg;
	char *response;
	int n;
	int clientCounter = 0;
	
	// Running this loop forever...
	while(1) {
	
		clientCounter++;
	 	clientFd = getClientFd(sockFd);  // Waiting for the client to connect.
	 	
		n = read(clientFd, buff, MAX_BUFF - 1); // Reading what client has to say.
		printf("%s\r\n", buff); // Printing the message received from the client.
	
		msg = (char *)calloc(MAX_BUFF, sizeof(char));
		headers = (char *)calloc(MAX_BUFF, sizeof(char));
		sprintf(msg, "Hi, Client number %d!\r\n", clientCounter);
		sprintf(headers, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %lu\r\n\r\n", strlen(msg));
		
		response = (char *)calloc(MAX_BUFF + MAX_BUFF, sizeof(char));
		response = strcat(headers, msg);
		n = write(clientFd, response, strlen(response)); // Writing a response message for the client
		
		close(clientFd); // Closing the client connection.
	}
		
	close(sockFd);
	return 0;
}
