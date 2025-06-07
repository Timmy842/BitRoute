#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Conectado al servidor. Escribe comandos (HELLO, TIME, ECHO <texto>, FILE <archivo>):\n");

    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(sockfd, buffer, strlen(buffer));

        int bytes_read = read(sockfd, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) break;

        buffer[bytes_read] = '\0';
        printf("Respuesta: %s", buffer);

        // Si el servidor manda EOF en FILE, seguir leyendo hasta que llegue.
        if (strncmp(buffer, "LINE", 4) == 0 || strncmp(buffer, "EOF", 3) == 0) {
            while (strncmp(buffer, "EOF", 3) != 0) {
                bytes_read = read(sockfd, buffer, BUFFER_SIZE - 1);
                buffer[bytes_read] = '\0';
                printf("%s", buffer);
            }
        }
    }

    close(sockfd);
    return 0;
}
