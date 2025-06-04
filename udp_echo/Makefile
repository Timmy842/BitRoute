# Nombre de los ejecutables
CLIENT = client_udp
SERVER = server_udp

# Compilador y flags
CC = gcc
CFLAGS = -Wall -O2

# Reglas por defecto
all: $(CLIENT) $(SERVER)

# Compilación del cliente
$(CLIENT): client_udp.c
	$(CC) $(CFLAGS) -o $(CLIENT) client_udp.c

# Compilación del servidor
$(SERVER): server_udp.c
	$(CC) $(CFLAGS) -o $(SERVER) server_udp.c

# Limpiar archivos compilados
clean:
	rm -f $(CLIENT) $(SERVER)
