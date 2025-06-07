#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

char* get_current_time() {
  static char buffer[64];
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
  return buffer;
}

void send_file(const char *filename, int client_fd) {
  FILE *file = fopen(filename, "r");
  char line[256];

  if (!file) {
    char *error = "ERROR File not found\n";
    write(client_fd, error, strlen(error));
    return;
  }

  while (fgets(line, sizeof(line), file)) {
    char send_line[300];
    snprintf(send_line, sizeof(send_line), "LINE %s", line);
    write(client_fd, send_line, strlen(send_line));
  }

  char *eof = "EOF\n";
  write(client_fd, eof, strlen(eof));

  fclose(file);
}
