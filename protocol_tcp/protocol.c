#include "protocol.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void process_command(const char *input, int client_fd, const char *client_ip, int client_port) {
  char buffer[BUFFER_SIZE];

  if (strncmp(input, "HELLO", 5) == 0) {
    snprintf(buffer, sizeof(buffer), "Hello %s:%d\n", client_ip, client_port);
  }
  else if (strncmp(input, "TIME", 4) == 0) {
    char *time_str = get_current_time();
    snprintf(buffer, sizeof(buffer), "TIME %s\n", time_str);
  }
  else if (strncmp(input, "ECHO ", 5) == 0) {
    snprintf(buffer, sizeof(buffer), "ECHO %s\n", input + 5);
  }
  else if (strncmp(input, "FILE ", 5) == 0) {
    const char *filename = input + 5;
    send_file(filename, client_fd);
    return;
  }
  else {
    snprintf(buffer, sizeof(buffer), "ERROR Unknown command\n");
  }

  write(client_fd, buffer, strlen(buffer));
}
