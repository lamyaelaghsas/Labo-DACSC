#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int Socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

int Bind(int sockfd, const char* ip, int port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    return bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
}

int Listen(int sockfd, int backlog) {
    return listen(sockfd, backlog);
}

int Accept(int sockfd) {
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    return accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
}

int Connect(int sockfd, const char* ip, int port) {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip);

    return connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
}

int Send(int sockfd, const void* buffer, int length) {
    return send(sockfd, buffer, length, 0);
}

int Receive(int sockfd, void* buffer, int length) {
    return recv(sockfd, buffer, length, 0);
}

int Close(int sockfd) {
    return close(sockfd);
}
