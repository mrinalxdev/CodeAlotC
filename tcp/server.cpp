#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&address)) < 0){
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    char buffer[1024] = {0};
    read(new_socket, buffer, 1024); 
    std::cout << "Received Messages: " << buffer << std::endl;

    const char* message = "Hello from server!";
    send(new_socket, message, strlen(message), 0);

    return 0;
}