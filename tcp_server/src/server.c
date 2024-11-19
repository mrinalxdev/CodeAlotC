#include "server.h"
#include <stdio.h>

int initialize_server(void){
    int server_fd;
    struct sockaddr_in address;
    int opt = 1; \

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket creation failed");
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
        perror("Setsocketop failed");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("Bind failed");
        return -1;
    }

    if (listen(server_fd, 3) < 0){
        perror("Listen Failed");
        return -1;
    }

    return server_fd;
}

int accept_connection(int server_fd){
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int client_fd;

    if ((client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
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

void close_server(int server_fd){
    close(server_fd);
}