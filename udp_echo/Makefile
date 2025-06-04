# Makefile
CC = gcc
CFLAGS = -Wall

all: server client

server: server.c
	$(CC) $(CFLAGS) -o server server_udp.c

client: client.c
	$(CC) $(CFLAGS) -o client client_udp.c

clean:
	rm -f server client
