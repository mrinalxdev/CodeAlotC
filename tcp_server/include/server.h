#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <signal.h>
#include <stdbool.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_TIMEOUT_SEC 10

typedef enum
{
    SERVER_INIT_SUCCESS,
    SERVER_INIT_SOCKET_FAIL,
    SERVER_INIT_SOCKOPT_FAIL,
    SERVER_INIT_BIND_FAIL,
    SERVER_INIT_LISTEN_FAIL,
} ServerInitResult;

typedef struct
{
    int server_fd;
    volatile sig_atomic_t keep_running;
    bool is_test_mode;
} ServerState;

ServerInitResult initialize_server(ServerState *server_state);
int accept_connection(int server_fd, int timeout_sec);
int handle_client(int client_fd);
void close_server(ServerState *server_state);

#endif