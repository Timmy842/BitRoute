#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netinet/in.h>

#define BUF_SIZE 8192

const char *get_mime_type(const char *path) {
    const char *ext = strrchr(path, '.');
    if (!ext) return "application/octet-stream";
    ext++;

    if (strcmp(ext, "html") == 0) return "text/html";
    if (strcmp(ext, "css") == 0) return "text/css";
    if (strcmp(ext, "js") == 0) return "application/javascript";
    if (strcmp(ext, "png") == 0) return "image/png";
    if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, "gif") == 0) return "image/gif";
    if (strcmp(ext, "txt") == 0) return "text/plain";
    return "application/octet-stream";
}

void send_response(int client_fd, int status, const char *status_text, const char *content_type, const void *body, size_t body_len, int keep_alive) {
    char header[BUF_SIZE];
    int len = snprintf(header, sizeof(header),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: %s\r\n"
        "\r\n",
        status, status_text, content_type, body_len,
        keep_alive ? "keep-alive" : "close");

    write(client_fd, header, len);
    write(client_fd, body, body_len);
}

void send_error(int client_fd, int status, const char *status_text, int keep_alive) {
    char body[BUF_SIZE];
    int len = snprintf(body, sizeof(body),
        "<html><body><h1>%d %s</h1></body></html>", status, status_text);

    send_response(client_fd, status, status_text, "text/html", body, len, keep_alive);
}

void handle_client(int client_fd) {
    char buffer[BUF_SIZE];
    int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) return;

    buffer[bytes_read] = '\0';
    printf("Petición recibida:\n%s\n", buffer);

    // Parseo de la primera línea
    char method[16], path[1024], version[16];
    sscanf(buffer, "%15s %1023s %15s", method, path, version);

    // Parseo de cabeceras (muy simple)
    int keep_alive = 0;
    char *header_line = strstr(buffer, "\r\n") + 2;
    while (header_line && *header_line != '\r' && *header_line != '\n') {
        if (strncasecmp(header_line, "Connection:", 11) == 0) {
            char connection_value[64];
            sscanf(header_line + 11, "%63s", connection_value);
            if (strcasecmp(connection_value, "keep-alive") == 0) {
                keep_alive = 1;
            }
        }
        header_line = strstr(header_line, "\r\n");
        if (header_line) header_line += 2;
    }

    // Permitir GET y POST
    if (strcmp(method, "GET") != 0 && strcmp(method, "POST") != 0) {
        send_error(client_fd, 405, "Method Not Allowed", keep_alive);
        return;
    }

    // Si es POST, buscar Content-Length y leer el cuerpo
    char *body_ptr = NULL;
    int content_length = 0;
    if (strcmp(method, "POST") == 0) {
        char *cl = strstr(buffer, "Content-Length:");
        if (cl) {
            sscanf(cl, "Content-Length: %d", &content_length);
        }
        // Buscar el inicio del cuerpo (después de la doble nueva línea)
        body_ptr = strstr(buffer, "\r\n\r\n");
        if (body_ptr) body_ptr += 4;
        else body_ptr = strstr(buffer, "\n\n");
        if (body_ptr) body_ptr += 2;
        // Si el cuerpo no está completo en el buffer, leer el resto
        int body_bytes = bytes_read - (body_ptr - buffer);
        char *post_body = malloc(content_length + 1);
        if (post_body) {
            memcpy(post_body, body_ptr, body_bytes);
            int remaining = content_length - body_bytes;
            int offset = body_bytes;
            while (remaining > 0) {
                int n = read(client_fd, post_body + offset, remaining);
                if (n <= 0) break;
                offset += n;
                remaining -= n;
            }
            post_body[content_length] = '\0';
            printf("Cuerpo POST recibido (%d bytes):\n%s\n", content_length, post_body);
            // Responder con confirmación
            const char *resp = "<html><body><h1>POST recibido</h1></body></html>";
            send_response(client_fd, 200, "OK", "text/html; charset=UTF-8", resp, strlen(resp), keep_alive);
            free(post_body);
            return;
        } else {
            send_error(client_fd, 500, "Internal Server Error", keep_alive);
            return;
        }
    }

    if (strcmp(path, "/") == 0) {
        strcpy(path, "/index.html");
    }

    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "www%s", path);

    int fd = open(full_path, O_RDONLY);
    if (fd < 0) {
        send_error(client_fd, 404, "Not Found", keep_alive);
        return;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        close(fd);
        send_error(client_fd, 500, "Internal Server Error", keep_alive);
        return;
    }

    void *file_content = malloc(st.st_size);
    if (!file_content) {
        close(fd);
        send_error(client_fd, 500, "Internal Server Error", keep_alive);
        return;
    }

    read(fd, file_content, st.st_size);
    close(fd);

    const char *mime_type = get_mime_type(full_path);
    send_response(client_fd, 200, "OK", mime_type, file_content, st.st_size, keep_alive);

    free(file_content);
}
