#ifndef SERVER_H
#define SERVER_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int initialize_server(void);
int accept_connection(int server_fd);
int handle_client(int client_fd);
void close_server(int server_fd);

#endif