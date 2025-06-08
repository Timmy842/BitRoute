# Makefile
CC=gcc
CFLAGS=-Wall -Wextra -O2
TARGET=http_server

all: $(TARGET)

$(TARGET): server.o http.o
	$(CC) $(CFLAGS) -o $(TARGET) server.o http.o

server.o: server.c http.h
	$(CC) $(CFLAGS) -c server.c

http.o: http.c http.h
	$(CC) $(CFLAGS) -c http.c

clean:
	rm -f *.o $(TARGET)
