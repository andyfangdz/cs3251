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

CXXFLAGS=-std=c++0x
LIBS=-lcrypto

OS := $(shell uname -s)

# Extra LDFLAGS if Solaris
ifeq ($(OS), SunOS)
	LDFLAGS=-lsocket -lnsl
endif

all: client server

client: client.cpp
	$(CXX) client.cpp -o $@ $(CXXFLAGS) $(LIBS)

server: server.cpp
	$(CXX) server.cpp -o $@ $(CXXFLAGS) $(LIBS)

clean:
	rm -f client server nameChanger changeServer *.o

