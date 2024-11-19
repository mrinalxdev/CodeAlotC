#include <assert.h>
#include "server.h"

void test_server_initialisation(void){
    int server_fd = intilialise_server();
    assert(server_fd > 0);
    close_server(server_fd);
    printf("Server initialisation test passed\n");
}

int test_client_connection(void){
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *message = "Hello from client";
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
        printf("\n Invalid address/ Address not supported \n");
        return -1;
    }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\n Connection Failed \n");
        return -1;
    }
    

}