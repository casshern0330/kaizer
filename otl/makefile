#HOME = /home/casshern
UNPHOME = /cygdrive/d/library/unpv13e
INCLUDE = -I$(UNPHOME)/include -I/usr/include
LIBS = -L$(UNPHOME) -lunp -lm
CFLAGS = -g -O2 -Wall $(INCLUDE)
BINDIR=./bin

CC = gcc

all:hello client server

mysql_smp01:mysql_smp01.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $?

clean:
	rm -rf *.o
