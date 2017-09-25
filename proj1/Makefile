#################################################################
##
## FILE:	Makefile
## PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura
## DESCRIPTION: Compile Project 1
## CREDIT:	Adapted from Professor Traynor
##
#################################################################

CXXFLAGS=-std=c++0x -g -O3

OS := $(shell uname -s)

# Extra LDFLAGS if Solaris
ifeq ($(OS), SunOS)
LDFLAGS=-lsocket -lnsl $(LDFLAGS)
endif

all: client server

client: client.cpp

server: server.cpp

clean:
	rm -rf client server *.o *.dSYM

clean-clion: clean
	rm -rf cmake-build-debug

test:
	python test.py
