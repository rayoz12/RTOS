#
#  	----- 48450 -- week 2 lab handout 8-4 ------ 
#	In order to learn a basic concept about how a makefile to # compiling a source code into an object/#execute file. Let us # run this example and go through the code lines in both the c # file and makefile.  


#Makefile

CC = gcc
CFLAGS = -Wall -O2


all: hi bye clean clean

P8-1-main.o: P8-1-main.c P8-1-main.h
	$(CC) -c P8-1-main.c 
hi:P8-2-hi.o P8-1-main.o 
	$(CC) -o hi P8-2-hi.o P8-1-main.o
bye:P8-3-bye.o P8-1-main.o
	$(CC) -o bye P8-3-bye.o P8-1-main.o


clean:
	rm -rf P8*.o core
