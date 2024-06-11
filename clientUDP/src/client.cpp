#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#include "../include/client.hpp"


namespace udp
{

void client::create_socket_desc()
{
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "socket creation failed" << std::endl;
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
}

void client::server_config()
{
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("192.168.0.131"); // Use localhost for testing
}

void client::send_msg()
{
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
}

void client::recieve()
{
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
}

} // namespace udp


int main()
{
    udp::client client_hello;


    client_hello.create_socket_desc();
    client_hello.server_config();
    client_hello.send_msg();
    client_hello.recieve();

    return 0;
}
