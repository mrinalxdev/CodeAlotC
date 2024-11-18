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
}