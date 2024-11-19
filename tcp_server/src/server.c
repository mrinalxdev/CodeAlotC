#include "../include/server.h"
#include <errno.h>

static ServerState *g_server_state = NULL;

void signal_handler(int signum){
    if (g_server_state != NULL){
        g_server_state -> keep_running = 0;
        printf("\nReceived signal %d. Shutting down server .. \n", signum);
    }
}

void setup_signal_handlers(ServerState *server_state){
    g_server_state = server_state;
    struct sigaction sa;

    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1){
        perror("Error : cannot handle SIGINT");
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1){
        perror("Error : cannot handle SIGTERM");
    }
}

ServerInitResult initialize_server(ServerState *server_state) {
    struct sockaddr_in address;
    int opt = 1;
    server_state->keep_running = 1;

    // TODO : Creating a socket
    server_state->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_state->server_fd < 0) {
        perror("Socket creation failed");
        return SERVER_INIT_SOCKET_FAIL;
    }

    if (setsockopt(server_state->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        close(server_state->server_fd);
        return SERVER_INIT_SOCKOPT_FAIL;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_state->server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_state->server_fd);
        return SERVER_INIT_BIND_FAIL;
    }
    if (listen(server_state->server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_state->server_fd);
        return SERVER_INIT_LISTEN_FAIL;
    }
    setup_signal_handlers(server_state);

    return SERVER_INIT_SUCCESS;
}

int accept_connection(int server_fd, int timeout_sec) {
    fd_set readfds;
    struct timeval timeout;
    int client_fd;
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);

    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;
    int ready = select(server_fd + 1, &readfds, NULL, NULL, &timeout);
    
    if (ready < 0) {
        perror("select() error");
        return -1;
    }
    
    if (ready == 0) {
        // Timeout occurred
        printf("Accept timeout after %d seconds\n", timeout_sec);
        return -1;
    }
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    
    if (client_fd < 0) {
        perror("Accept failed");
        return -1;
    }

    return client_fd; 
}

int handle_client(int client_fd){
    // character buffer to store incoming data
    char buffer[BUFFER_SIZE] = {0};
    int valread;
    /*
    attemting to read data from the client socket, utilizing 
    the read function to retrieve incoming data from the connected client where client_fd represents 
    the unique file descriptor of the of the client socket, buffer serves as the designated storage for incoming 
    data, and BUFFER_SIZE specefies the maximum number of bytes to be read from the socket,
    with actual number of bytes read stored in the valread variable.

    todo : shutting down the socket
    */
    valread = read(client_fd, buffer, BUFFER_SIZE);
    if (valread < 0){
        perror("Read failed");
        return -1;
    }
    printf("Recieved : %s\n", buffer);

    // 0 : flags for send operation
    if (send(client_fd, buffer, strlen(buffer), 0) < 0){
        perror("Send Failed");
        return -1;
    }

    return 0;
}

void close_server(ServerState *server_state) {
    if (server_state->server_fd > 0) {
        close(server_state->server_fd);
        server_state->server_fd = -1;
    }
}