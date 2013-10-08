#################################################################
##
## FILE:	Makefile
## PROJECT:	CS 3251 Project 2 - Professor Traynor
## DESCRIPTION: Compile Project 2
##
#################################################################

CC=gcc

OS := $(shell uname -s)

# Extra LDFLAGS if Solaris
ifeq ($(OS), SunOS)
	LDFLAGS=-lsocket -lnsl
    endif

all: client server 

client: music_client.c
	$(CC) music_client.c -o music_client

server: music_server.c
	$(CC) music_server.c -o music_server -lcrypto -lpthread

clean:
	    rm -f client server *.o

