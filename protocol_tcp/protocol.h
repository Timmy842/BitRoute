#ifndef PROTOCOL_H
#define PROTOCOL_H

void process_command(const char *input, int client_fd, const char *client_ip, int client_port);

#endif // PROTOCOL_H
