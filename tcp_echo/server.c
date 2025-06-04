#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 2025
#define BUFFER_SIZE 1024

int main() {
  int server_fd, client_fd;
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  char buffer[BUFFER_SIZE];

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 5) < 0) {
    perror("Listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

   printf("Servidor escuchando en el puerto %d...\n", PORT);

  while (1) {
    client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (client_fd < 0) {
      perror("Accept failed");
      continue;
    }

    printf("Cliente conectado.\n");

    int n;
    while((n = read(client_fd, buffer, BUFFER_SIZE)) > 0) {
      write(client_fd, buffer, n);
      memset(buffer, 0, BUFFER_SIZE); // Limpiar el buffer
    }

    if (n < 0) {
      perror("Read failed");
    }

    printf("Cliente desconectado.\n");
    close(client_fd);
  }

  close(server_fd);
  return 0;
}