#include "../include/server.hpp"

namespace bridge
{

int server::init_port()
{
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Filling server information
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Server is running on port " << PORT << std::endl;

    return 0;
}

int server::receive_msg()
{
    double received_value;

    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, BUFSIZE, 0, (struct sockaddr *)&cliaddr, &len);

        buffer[n] = '\0'; // Null-terminate the received data
        std::cout << "Received: " << buffer << std::endl;

        received_value = hex2double(buffer);

        std::cout << received_value << std::endl;
    }

    close(sockfd);

    return 0;
}

double server::hex2double(const std::string &msg)
{
    union {
        double d;
        uint8_t bytes[sizeof(double)];
    } doubleUnion;

    // Parse the hex string back into bytes
    for (size_t i = 0; i < sizeof(double); ++i) {
        std::string byteString = msg.substr(i * 2, 2);
        doubleUnion.bytes[i] = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
    }

    return doubleUnion.d;
}

} // namespace bridge


int main()
{
    bridge::server bridge_server;

    int status = 0;

    status = bridge_server.init_port();
    status = bridge_server.receive_msg();

    return 0;
}
