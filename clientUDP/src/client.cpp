#include "../include/client.hpp"

namespace bridge
{

int client::init_port()
{
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    return 0;
}

int client::send_msg()
{
    std::string msgStr = double2hex(msg);

    sendto(sockfd, msgStr.c_str(), msgStr.size(), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    std::cout << "Message sent to the server." << std::endl;

    close(sockfd);
    return 0;

}

std::string client::double2hex(double val)
{
    union {
        double d;
        uint8_t bytes[sizeof(double)];
    } doubleUnion;

    doubleUnion.d = val;

    std::stringstream ss;
    for (size_t i = 0; i < sizeof(double); ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(doubleUnion.bytes[i]);
    }

    std::string hexStringMsg = ss.str();

    return hexStringMsg;
}

} // namespace bridge


int main()
{
    bridge::client bridge_client;

    int status = 0;

    status = bridge_client.init_port();
    status = bridge_client.send_msg();

    return 0;
}
