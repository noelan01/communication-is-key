#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#include "../include/server.hpp"


namespace udp
{
void server::init_server()
{
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "socket creation failed" << std::endl;
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cout << "bind failed" << std::endl;
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void server::recieve_data()
{
    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(
            sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
            (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom error");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';
        std::cout << "Client: " << buffer << std::endl;

        const char * hello = "Hello from server";
        if (sendto(
              sockfd, hello, strlen(
                  hello), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len) < 0)
        {
            std::cout << "sendto error" << std::endl;
            perror("sendto error");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        std::cout << "Hello message sent." << std::endl;
    }

    close(sockfd);
}


} // namespace udp


int main()
{
    /*
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "socket creation failed" << std::endl;
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));


    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cout << "bind failed" << std::endl;
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }


    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(
            sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL,
            (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom error");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';
        std::cout << "Client: " << buffer << std::endl;

        const char * hello = "Hello from server";
        if (sendto(
              sockfd, hello, strlen(
                  hello), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len) < 0)
        {
            std::cout << "sendto error" << std::endl;
            perror("sendto error");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        std::cout << "Hello message sent." << std::endl;
    }
    close(sockfd);
    */

    udp::server server_1;

    server_1.init_server();
    server_1.recieve_data();

    return 0;
}
