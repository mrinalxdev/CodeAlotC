#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> 

int main(){
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    const char* message = "Hello from client";
    send(sock, message, strlen(message), 0);

    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    std::cout << "Recieved response : " << buffer << std::endl; 

    return 0; 
}