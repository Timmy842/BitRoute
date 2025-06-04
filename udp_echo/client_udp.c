#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2026
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 3
#define MAX_ATTEMPTS 3

int main() {
  int sockfd;
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE];
  socklen_t addr_len = sizeof(server_addr);
  struct timeval timeout;

  // Crear socket UDP
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Error al crear el socket");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
    perror("Error al convertir la dirección IP");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // Configurar timeout para recvfrom
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  // Mensaje a enviar
  printf("Introduce un mensaje: ");
  fgets(buffer, BUFFER_SIZE, stdin);

  int attempts = 0;
  int recived = 0;

  while (attempts < MAX_ATTEMPTS && !recived) {
    // Enviar mensaje al servidor
    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server_addr, addr_len);

    // Esperar respuesta del servidor
    int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                        (struct sockaddr *)&server_addr, &addr_len);
    
    if (recv_len >= 0) {
      buffer[recv_len] = '\0';
      printf("Respuesta del servidor: %s\n", buffer);
      recived = 1;
    } else {
      printf("No se recibió respuesta, reintentando (%d/%d)...\n", ++attempts, MAX_ATTEMPTS);
      sleep(TIMEOUT_SEC);
    }
  }

  if (!recived) {
    printf("No se recibió respuesta del servidor después de %d intentos.\n", MAX_ATTEMPTS);
  }

  close(sockfd);
  return 0;
}