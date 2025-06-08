// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 2017
#define BUF_SIZE 1024

void recv_response(int sock) {
    char buffer[BUF_SIZE];
    int bytes = recv(sock, buffer, BUF_SIZE - 1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    recv_response(sock); // LOGIN:

    char userpass[100];
    printf("Enter username and password: ");
    fgets(userpass, sizeof(userpass), stdin);
    userpass[strcspn(userpass, "\n")] = '\0';

    send(sock, userpass, strlen(userpass), 0);
    recv_response(sock);

    char command[200];
    while (1) {
        printf("Enter command (LIST, GET filename, UP filename, QUIT): ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        send(sock, command, strlen(command), 0);

        if (strncmp(command, "LIST", 4) == 0 || strncmp(command, "GET ", 4) == 0) {
            recv_response(sock);
        } else if (strncmp(command, "UP ", 3) == 0) {
            char filename[100];
            sscanf(command + 3, "%s", filename);

            FILE *fp = fopen(filename, "rb");
            if (!fp) {
                printf("Cannot open file.\n");
                continue;
            }

            recv_response(sock); // READY

            char buffer[BUF_SIZE];
            int n;
            while ((n = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
                send(sock, buffer, n, 0);
            }
            fclose(fp);

            // Enviar EOF
            send(sock, "EOF", 3, 0);

            recv_response(sock); // UPLOAD COMPLETE
        } else if (strncmp(command, "QUIT", 4) == 0) {
            break;
        } else {
            recv_response(sock);
        }
    }

    close(sock);
    return 0;
}
