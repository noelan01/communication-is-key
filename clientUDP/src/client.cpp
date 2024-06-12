#include <iostream>

#include "../include/client.hpp"

namespace udp
{
    int client::init_client()
    {
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed: " << result << std::endl;
            return 1;
        }

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        inet_pton(AF_INET, "192.168.0.104", &servaddr.sin_addr);

        return 0;
    }


    int client::send_msg()
    {
        const char *hello = "Hello from client";
        int len = sizeof(servaddr);

        int sendResult = sendto(sockfd, hello, strlen(hello), 0, (sockaddr *)&servaddr, len);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "sendto error: " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
        std::cout << "Hello message sent." << std::endl;

        char buffer[BUFFER_SIZE];
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (sockaddr *)&servaddr, &len);
        if (n == SOCKET_ERROR) {
            std::cerr << "recvfrom error: " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }

        buffer[n] = '\0';
        std::cout << "Server: " << buffer << std::endl;

        closesocket(sockfd);
        WSACleanup();

        return 0;
    }
    
} // namespace udp


int main() {
    /*
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "192.168.0.104", &servaddr.sin_addr);

    const char *hello = "Hello from client";
    int len = sizeof(servaddr);

    int sendResult = sendto(sockfd, hello, strlen(hello), 0, (sockaddr *)&servaddr, len);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "sendto error: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    std::cout << "Hello message sent." << std::endl;

    char buffer[BUFFER_SIZE];
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (sockaddr *)&servaddr, &len);
    if (n == SOCKET_ERROR) {
        std::cerr << "recvfrom error: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    buffer[n] = '\0';
    std::cout << "Server: " << buffer << std::endl;

    closesocket(sockfd);
    WSACleanup();
    */
    udp::client client_1;

    int flag = client_1.init_client();
    flag = client_1.send_msg();

    if (flag == 1)
    {
        std::cout << "something went wrong" << std::endl;
    }

    return 0;
}
