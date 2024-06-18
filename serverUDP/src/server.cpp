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

        std::cout << n << std::endl;
        /*
        if (n != sizeof(double)) {
            std::cerr << "Received data of incorrect size" << std::endl;
            continue;
        }
        */

        memcpy(&received_value, buffer, sizeof(double));

        std::cout << "Received: " << received_value << std::endl;
    }

    close(sockfd);

    return 0;
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
