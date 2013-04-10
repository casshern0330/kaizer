HOME = /home/hucw
INCLUDE = -I/usr/include -I$(HOME)/include
LIBS = -lm -L$(HOME)/lib -lunp
CFLAGS = -g -O2 -Wall $(INCLUDE)
BINDIR=./bin

CC = gcc

all:hello client server

getWeb:getWeb.o
	rm -rf $(BINDIR)/$@
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)
clean:
	rm -rf *.o
