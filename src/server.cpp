#include <cstring>
#include <arpa/inet.h>
#include <sstream>

#include "../include/server.hpp"


namespace udp
{
void server::init_server()
{
    //std::cout << socket(AF_INET, SOCK_DGRAM, 0) << std::endl;
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

        std::string msgStr = buffer;

        parse_msg(msgStr);

        std::cout << recv_id << std::endl;
        std::cout << recv_data << std::endl;
        /*
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
        */
    }

    close(sockfd);
}

void server::parse_msg(std::string msg)
{
    std::string idStr = msg.substr(0,2);
    msg.erase(0,2);

    std::stringstream ss;
    ss << idStr;
    ss >> recv_id;
    recv_id = recv_id - 10;

    union {
        double d;
        uint8_t bytes[sizeof(double)];
    } doubleUnion;

    // Parse the hex string back into bytes
    for (size_t i = 0; i < sizeof(double); ++i) {
        std::string byteString = msg.substr(i * 2, 2);
        doubleUnion.bytes[i] = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
    }

    recv_data = doubleUnion.d;
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
