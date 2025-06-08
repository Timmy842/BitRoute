// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>

#define PORT 2017
#define BUF_SIZE 1024
#define USERS_FILE "users.txt"
#define FILES_DIR "files/"

typedef struct {
    char username[50];
    char password[50];
} User;

User users[10];
int user_count = 0;

void load_users() {
    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) {
        perror("users.txt");
        exit(1);
    }
    while (fscanf(fp, "%s %s", users[user_count].username, users[user_count].password) != EOF) {
        user_count++;
    }
    fclose(fp);
}

int authenticate(const char *username, const char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
            return 1;
        }
    }
    return 0;
}

void handle_client(int client_sock) {
    char buffer[BUF_SIZE];
    char username[50], password[50];

    send(client_sock, "LOGIN:\n", 7, 0);

    int bytes = recv(client_sock, buffer, BUF_SIZE - 1, 0);
    if (bytes <= 0) return;
    buffer[bytes] = '\0';

    sscanf(buffer, "%s %s", username, password);

    if (authenticate(username, password)) {
        send(client_sock, "OK\n", 3, 0);
    } else {
        send(client_sock, "FAIL\n", 5, 0);
        close(client_sock);
        return;
    }

    while (1) {
        bytes = recv(client_sock, buffer, BUF_SIZE - 1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';

        if (strncmp(buffer, "LIST", 4) == 0) {
            DIR *dir = opendir(FILES_DIR);
            struct dirent *entry;
            char list[BUF_SIZE] = "";
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_type == DT_REG) {
                    strcat(list, entry->d_name);
                    strcat(list, "\n");
                }
            }
            closedir(dir);
            send(client_sock, list, strlen(list), 0);
        } else if (strncmp(buffer, "GET ", 4) == 0) {
            char filename[100];
            sscanf(buffer + 4, "%s", filename);

            if (strstr(filename, "..") != NULL) {
                send(client_sock, "ERROR: Invalid filename.\n", 25, 0);
                continue;
            }

            char path[150];
            snprintf(path, sizeof(path), "%s%s", FILES_DIR, filename);

            FILE *fp = fopen(path, "rb");
            if (!fp) {
                send(client_sock, "ERROR: File not found.\n", 23, 0);
                continue;
            }

            while (1) {
                int n = fread(buffer, 1, BUF_SIZE, fp);
                if (n > 0) send(client_sock, buffer, n, 0);
                if (n < BUF_SIZE) break;
            }
            fclose(fp);
        } else if (strncmp(buffer, "UP ", 3) == 0) {
            char filename[100];
            sscanf(buffer + 3, "%s", filename);

            if (strstr(filename, "..") != NULL) {
                send(client_sock, "ERROR: Invalid filename.\n", 25, 0);
                continue;
            }

            char path[150];
            snprintf(path, sizeof(path), "%s%s", FILES_DIR, filename);

            FILE *fp = fopen(path, "wb");
            if (!fp) {
                send(client_sock, "ERROR: Cannot open file for writing.\n", 36, 0);
                continue;
            }

            send(client_sock, "READY\n", 6, 0);

            // Recibir bloques de datos
            while (1) {
                bytes = recv(client_sock, buffer, BUF_SIZE, 0);
                if (bytes <= 0) break;

                // Protocolo: cliente envía "EOF" para terminar
                if (strncmp(buffer, "EOF", 3) == 0) break;

                fwrite(buffer, 1, bytes, fp);
            }

            fclose(fp);
            send(client_sock, "UPLOAD COMPLETE\n", 16, 0);
        } else if (strncmp(buffer, "QUIT", 4) == 0) {
            break;
        } else {
            send(client_sock, "ERROR: Unknown command.\n", 24, 0);
        }
    }

    close(client_sock);
}

int main() {
    load_users();

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        printf("Client connected.\n");

        if (fork() == 0) {
            close(server_sock);
            handle_client(client_sock);
            exit(0);
        } else {
            close(client_sock);
        }
    }

    close(server_sock);
    return 0;
}
