#################################################################
##
## FILE:	Makefile
## PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura
## DESCRIPTION: Compile Project 1
## CREDIT:	Adapted from Professor Traynor
##
#################################################################

CC=gcc
CXX=g++

OS := $(shell uname -s)

# Extra LDFLAGS if Solaris
ifeq ($(OS), SunOS)
	LDFLAGS=-lsocket -lnsl
endif

all: client server

client: client.cpp
	$(CXX) client.cpp -o nameChanger

server: server.cpp
	$(CXX) -lcrypto server.cpp -o changeServer

clean:
	rm -f client server nameChanger changeServer *.o

