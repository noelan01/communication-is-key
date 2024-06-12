#include <iostream>

#include "../include/server.hpp"

namespace udp
{

    int server::init_server()
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
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(PORT);

        if (bind(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
        return 0;   // return 1 if error
    }

    int server::recieve()
    {
        while (true) 
        {
            int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (sockaddr *)&cliaddr, &len);
            if (n == SOCKET_ERROR) {
                std::cerr << "recvfrom error: " << WSAGetLastError() << std::endl;
                closesocket(sockfd);
                WSACleanup();
                return 1;
            }

            buffer[n] = '\0';
            std::cout << "Client: " << buffer << std::endl;

            const char *hello = "Hello from server";
            int sendResult = sendto(sockfd, hello, strlen(hello), 0, (sockaddr *)&cliaddr, len);
            if (sendResult == SOCKET_ERROR) {
                std::cerr << "sendto error: " << WSAGetLastError() << std::endl;
                closesocket(sockfd);
                WSACleanup();
                return 1;
            }

            std::cout << "Hello message sent." << std::endl;
        }

        closesocket(sockfd);
        WSACleanup();
        return 0; // return 1 if error
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
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    char buffer[BUFFER_SIZE];
    sockaddr_in cliaddr;
    int len = sizeof(cliaddr);
    

    while (true) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (sockaddr *)&cliaddr, &len);
        if (n == SOCKET_ERROR) {
            std::cerr << "recvfrom error: " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }

        buffer[n] = '\0';
        std::cout << "Client: " << buffer << std::endl;

        const char *hello = "Hello from server";
        int sendResult = sendto(sockfd, hello, strlen(hello), 0, (sockaddr *)&cliaddr, len);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "sendto error: " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }

        std::cout << "Hello message sent." << std::endl;
    }

    closesocket(sockfd);
    WSACleanup();
    */
   
    udp::server server_1;

    int flag = server_1.init_server();
    flag = server_1.recieve();

    if (flag == 1)
    {
        std::cout << "something went wrong" << std::endl;
    }

    return 0;
}
