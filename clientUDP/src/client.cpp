#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    char buffer[BUFFER_SIZE];
    const char * hello = "Hello from client";
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "socket creation failed" << std::endl;
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("192.168.0.131"); // Use localhost for testing

    int n, len;

    if (sendto(
          sockfd, hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *)&servaddr,
          sizeof(servaddr)) < 0)
    {
        std::cout << "sendto error" << std::endl;
        perror("sendto error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    std::cout << "Hello message sent." << std::endl;

    n =
      recvfrom(
        sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr,
        (socklen_t *)&len);
    if (n < 0) {
        std::cout << "recvfrom error" << std::endl;
        perror("recvfrom error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    std::cout << "Server: " << buffer << std::endl;

    close(sockfd);
    return 0;
}
