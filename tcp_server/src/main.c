#include "server.h"

int main(void){
    int server_fd, client_fd;
    server_fd = initialize_server();

    if(server_fd < 0){
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while(1){
        client_fd = accept_connection(server_fd);
        if(client_fd < 0){
            continue;
        }

        if(handle_client(client_fd) < 0){
            close(client_fd);
            continue;
        }

        close(client_fd);
    }

    close_server(server_fd);
    return 0;
}