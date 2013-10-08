/*///////////////////////////////////////////////////////////
*
* FILE:     server.c
* AUTHOR:   Your Name Here
* PROJECT:  CS 3251 Project 1 - Professor Traynor
* DESCRIPTION:  Network Server Code
*
*////////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>    /* for printf() and fprintf() */
#include <sys/socket.h>   /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>   /* supports all sorts of functionality */
#include <unistd.h>   /* for close() */
#include <string.h>   /* support any string ops */
#include <openssl/evp.h>  /* for OpenSSL EVP digest libraries/SHA256 */
#include <pthread.h>
#include <fcntl.h>
#include <dirent.h>
#include <openssl/md5.h>
#include <errno.h>


#define RCVBUFSIZE 1000      /* The receive buffer size */
#define SNDBUFSIZE 1000      /* The send buffer size */
#define BUFSIZE 1000      /* Your name can be as many as 40 chars*/
#define REQSIZE 1       /* Request can only be one byte, 1- */
void list_songs(int clientSock);
void *ThreadMain(void *arg);

struct ThreadArgs{
    int clntSock;
};

typedef struct{
    char fileName[NAME_MAX+1];
    unsigned char hash[MD5_DIGEST_LENGTH];
}Song;

typedef struct{
    int size;
    Song **songs;
}SongArr;

/* The main function */
int main(int argc, char *argv[])
{

    int serverSock;             /* Server Socket */
    int clientSock;             /* Client Socket */
    struct sockaddr_in changeServAddr;      /* Local address */
    struct sockaddr_in changeClntAddr;      /* Client address */
    unsigned short changeServPort = 1111;       /* Server port */
    unsigned int clntLen;           /* Length of address data struct */

    char nameBuf[BUFSIZE];          /* Buff to store name from client */
    unsigned char md_value[EVP_MAX_MD_SIZE];    /* Buff to store change result */
    EVP_MD_CTX *mdctx;              /* Digest data structure declaration */
    const EVP_MD *md;               /* Digest data structure declaration */
    int md_len;                 /* Digest data structure size tracking */


    /* Create new TCP Socket for incoming requests*/
    /*      FILL IN */
    serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ((serverSock < 0)){
        printf("socket() failed");
        exit(1);
    }

    /* Construct local address structure*/
    /*      FILL IN */
    memset(&changeServAddr, 0, sizeof(changeServAddr));
    changeServAddr.sin_family = PF_INET;
    changeServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    changeServAddr.sin_port = htons(changeServPort);

    /* Bind to local address structure */
    /*      FILL IN */
    if (bind(serverSock, (struct sockaddr *) &changeServAddr, sizeof(changeServAddr)) <0)
        printf("bind() failed");

    /* Listen for incoming connections */
    /*      FILL IN */
    if (listen(serverSock, 10) < 0){
        puts("listen() failed");
    }

    /* Loop server forever*/
    while(1)
    {
        clntLen = sizeof(changeClntAddr);
        /* Accept incoming connection */
        /*  FILL IN     */
        int clientSock = accept(serverSock, (struct sockaddr *) &changeClntAddr, &clntLen);
        if (clientSock < 0)
            puts("accept() failed");

        /* Create Thread Args/separate memory for client argument*/
        struct ThreadArgs *threadArgs;
        threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
        if (threadArgs == NULL){
            puts("malloc() failed");
        }
        threadArgs->clntSock = clientSock;

        /* Create client thread */
        pthread_t threadID;
        int returnValue = pthread_create(&threadID, NULL, ThreadMain, threadArgs);
        if (returnValue != 0){
            printf("with thread %lu\n", (unsigned long int) threadID);
        }
    }

    //send(clientSock, md_value, md_len, 0);
}

void *ThreadMain(void *ThreadArgs){
    int clientSock;
    unsigned char reqBuf[REQSIZE];
    int command;
    
    memset(&reqBuf, 0, sizeof(reqBuf));
    
    //Guarantees that thread resources are deallocated upon return
    pthread_detach(pthread_self());

    //Extract socket file descriptor from argument
    clientSock = ((struct ThreadArgs *) ThreadArgs)->clntSock;
    free(ThreadArgs);//Deallocate memory for argument

    /*Get client's request from sent bytes*/
    int recvBytes = recv(clientSock, reqBuf, REQSIZE, 0);
	reqBuf[REQSIZE] = '\0';
	command = reqBuf[0];
    if(recvBytes<0){
        printf("recv() failed");
    }
    
	//printf("ReqBuff: %d\n", reqBuf[0]);
    //printf("Server recv: %d\n", recvBytes);
    //printf("Command recv: %d\n", *command);
    
    if(command == 1){
        list_songs(clientSock);
        exit(0);
    }
    else if(command == 2){
        //diff fucntion
        //exit
    }
    else if(command == 3){
        //pull function
        //exit
    }
    else if(command == 4){
        //leave function
        //exit
    }
    
    
    return(NULL);
}

void list_songs(int clientSock){
    puts("\nList of songs in library:\n");

	/*Init Vars
	SongArr *song_list = malloc(sizeof(SongArr));
	song_list->size = 0;
	song_list->songs = malloc(sizeof(Song *));
	*/

	unsigned char sndBuf[RCVBUFSIZE];
	int size = 0;
    memset(&sndBuf, 0, RCVBUFSIZE);
	
	/*Scanning Directory*/
	DIR *dp;
	struct dirent *ep;     
	dp = opendir ("./music");
	

  
  
	if (dp != NULL){
		while (ep = readdir(dp)){
			if(strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0){
				int nameLength = strlen(ep->d_name);
				strcpy(sndBuf+size, ep->d_name);
				size += nameLength;
				sndBuf[size] = "|";
				size++;
			}
		}
    (void)closedir(dp);
	}
	
	printf("%d\n", size);
	
	int sentBytes = send(clientSock, sndBuf, size, 0);
    fprintf(stderr,"%s",strerror(errno));
	if(sentBytes < 0){
		puts("\n Nothing was sent!!\n");
	}
	printf("Server sent %d bytes for LIST\n", sentBytes);
	
	
	
	/*
	char a[counter][NAME_MAX+1];
	
	dp = opendir ("./music");
	counter = 0;
	if (dp != NULL){
		while (ep = readdir(dp)){
			if(strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0){
				strcpy(a[counter],ep->d_name);
				counter++;
			}
		}
    (void)closedir(dp);
	}
	*/
	
}
