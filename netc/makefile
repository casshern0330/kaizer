#HOME = /home/casshern
UNPHOME = /cygdrive/d/library/unpv13e
INCLUDE = -I$(UNPHOME)/include
LIBS = -L$(UNPHOME) -lunp
CFLAGS = -g -O2 -Wall $(INCLUDE)
BINDIR=./bin

CC = gcc

all:hello client server

hello:hello.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $?

client:client.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

server:server.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

#客户端程序,通过addrinfo获取
client01:client01.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

#服务器端程序,通过fork函数实现
server01:server01.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

#服务器端程序,通过select函数实现
server02:server02.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

#客户端程序，通过select函数实现
client02:client02.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

udpclient:udpclient.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

udpserver:udpserver.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

mygetaddrinfo:mygetaddrinfo.o
	rm -rf $(BINDIR)/$@
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

print_addr:print_addr.o
	rm -rf $(BINDIR)/$@
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $? $(LIBS)

clean:
	rm -rf *.o
