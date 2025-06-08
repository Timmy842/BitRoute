# Variables
CC = gcc
CFLAGS = -Wall -O2
TARGETS = server client

# Default target
all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) server.c -o server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

# Clean target
clean:
	rm -f $(TARGETS)

.PHONY: all clean
