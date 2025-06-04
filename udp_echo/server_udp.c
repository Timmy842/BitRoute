#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 2026
#define BUFFER_SIZE 1024

int main() {
  int sockfd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(client_addr);
  char buffer[BUFFER_SIZE];

  // Crear socket UDP
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Error al crear el socket");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(SERVER_PORT);

  // Enlazar el socket al puerto
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Error al enlazar el socket");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Servidor UDP escuchando en el puerto %d...\n", SERVER_PORT);

  while (1) {
    memset(buffer, 0, BUFFER_SIZE); // Limpiar el buffer
    // Recibir datos del cliente
    ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                                 (struct sockaddr *)&client_addr, &addr_len);
    if (recv_len < 0) {
      perror("Error al recibir datos");
      close(sockfd);
      exit(EXIT_FAILURE);
    }

    buffer[recv_len] = '\0';
    printf("Mensaje recibido: %s\n", buffer);

    // Enviar respuesta al cliente
    if (sendto(sockfd, buffer, recv_len, 0,
               (struct sockaddr *)&client_addr, addr_len) < 0) {
      perror("Error al enviar datos");
      close(sockfd);
      exit(EXIT_FAILURE);
    }
  }

  // Cerrar el socket
  close(sockfd);
  return 0;
}