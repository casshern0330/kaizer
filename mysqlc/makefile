#HOME = /home/casshern
TARHOME = /cygdrive/d/library/mysql5
INCLUDE = -I$(TARHOME)/include
#LIBS = -L$(TARHOME)/lib
#CFLAGS = -g -O2 -Wall -I/usr/include/mysql
CFLAGS = `mysql_config --cflags`
LIBS = `mysql_config --libs`
BINDIR=./bin

CC = gcc

all:hello client server

demo:demo.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

demo01:demo01.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

demo_time:demo_time.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)
clean:
	rm -rf *.o
