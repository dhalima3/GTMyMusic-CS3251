/*///////////////////////////////////////////////////////////
*
* FILE:		music_client.c
* AUTHOR:	Daryl Halima, Vraj Patel
* PROJECT:	CS 3251 Project 2 - Professor Traynor
* DESCRIPTION:	Network Client Code
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/evp.h>	    /* for OpenSSL EVP digest libraries/SHA256 */

/* Constants */
#define RCVBUFSIZE 1000		    /* The receive buffer size */
#define SNDBUFSIZE 1000		    /* The send buffer size */
#define MDLEN 32

/* The main function */
int main(int argc, char *argv[])
{

    int clientSock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* The server address */

   // char *studentName;		    /* Your Name */

    char sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    unsigned char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */
    
    int i;			    /* Counter Value */
	unsigned int *command;
	
	/* Simple Interface */
	printf("Welcome to gtMyMusic\n");
	printf("Enter the number for the following actions:\n");
	printf("  LIST - 1\n");
	printf("  DIFF - 2\n");
	printf("  PULL - 3\n");
	printf("  LEAVE - 4\n");
	
	scanf("%o", command);

    memset(&sndBuf, 0, RCVBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Create a new TCP socket*/
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock<0){
        printf("socket creation failed");
    }

    /* Construct the server address structure */
    memset(&serv_addr, 0, sizeof(serv_addr));
    in_port_t servPort = 1111;
    serv_addr.sin_port = htons(servPort);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    serv_addr.sin_family = AF_INET;



    /* Establish connection to the server */
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ){
        printf("Connection to server failed...");
    }

    
    /* Send the string to the server */
    //still have to place name into buffer
    //int nameLength = strlen(studentName);
    //printf("Sent: %d", nameLength);
    //ssize_t numbBytes = send(sock, studentName, nameLength, 0);
	ssize_t numbBytes = send(sock, command, 1, 0);
    if(numbBytes < 0){
        printf("Failed to send message.");
    }

    /* Receive and print response from the server */
    unsigned int totalRecBytes = 0;
    while(totalRecBytes < 32){
        numbBytes=recv(sock,rcvBuf, RCVBUFSIZE-1,0);
        if(numbBytes < 0){
            printf("rec fail client");
        }
        totalRecBytes += numbBytes;
    }
    
    //numbBytes=recv(sock,rcvBuf, RCVBUFSIZE-1,0);

    printf("Transformed input is: ");
    for(i = 0; i < MDLEN; i++) printf("%02x", rcvBuf[i]);
    printf("\n");

    return 0;
}
