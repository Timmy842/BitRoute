# Nombre del ejecutable
SERVER = server
CLIENT = client

# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -Wextra -g

# Archivos fuente
SRCS = protocol.c utils.c
HDRS = protocol.h

# Targets

all: $(SERVER) $(CLIENT)

$(SERVER): server.c $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(SERVER) server.c $(SRCS)

$(CLIENT): client.c $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(CLIENT) client.c $(SRCS)

# Limpiar archivos generados
clean:
	rm -f $(SERVER) $(CLIENT)

# Phony targets
.PHONY: all clean
