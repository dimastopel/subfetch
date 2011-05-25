
CC=gcc
CFLAGS=-c `xmlrpc-c-config --cflags`
LIBS=`xmlrpc-c-config client --libs`

all: subfetch

subfetch: main.o utils.o 
	$(CC) $(LIBS) main.o utils.o -o subfetch

main.o: main.c
	$(CC) $(CFLAGS) main.c

utils.o: utils.c
	$(CC) $(CFLAGS) utils.c

clean:
	rm -rf *.o subfetch



