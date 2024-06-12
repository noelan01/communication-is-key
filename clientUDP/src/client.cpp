#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
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
    return 0;
}
