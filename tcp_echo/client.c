#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 2025
#define BUFFER_SIZE 1024

int main() {
  int sock;
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE];

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Connection failed");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Conectado al servidor en %s:%d. Escribe un mensaje:\n", SERVER_IP, PORT);
  while (fgets(buffer, BUFFER_SIZE, stdin)) {
    write(sock, buffer, strlen(buffer));
    memset(buffer, 0, BUFFER_SIZE); // Limpiar el buffer

    int n = read(sock, buffer, BUFFER_SIZE);
    
    if (n < 0) {
      perror("Read failed");
      break;
    } else if (n == 0) {
      printf("Servidor desconectado.\n");
      break;
    } else {
      buffer[n] = '\0'; // Asegurar que el buffer es una cadena
      printf("Respuesta del servidor: %s", buffer);
    }
  }

  close(sock);
  return 0;
}