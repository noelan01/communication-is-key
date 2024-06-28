#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <iomanip>

#include "../include/client.hpp"


namespace udp
{

void client::init()
{
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "socket creation failed" << std::endl;
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(serverIP);  // server IP (localhost)
}


void client::send_msg()
{
    init_vectors();

    for (int id = 0; id <= LAST_EL_SIMOUT; id++)
    {
        parse_msg(id, sim_out[id]);

        strcpy(&msg[0], StringMsg.data());        

        if (sendto(
            sockfd, &msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0)
        {
            std::cout << "sendto error" << std::endl;
            perror("sendto error");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

    }

    close(sockfd);
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

void client::init_vectors()
{
    for (int i = FIRST_EL_SIMIN; i <= LAST_EL_SIMIN; i++) {
        sim_in[i] = 0.0;
    }

    for (int i = FIRST_EL_SIMOUT; i <= LAST_EL_SIMOUT; i++) {
        sim_out[i] = 1.0;
    }
}

void client::parse_msg(int id, double data)
{
    id = id + 10;
    std::stringstream ssID;
    ssID << id;
    std::string idStr = ssID.str();

    union {
        double d;
        uint8_t bytes[sizeof(double)];
    } doubleUnion;

    doubleUnion.d = data;

    std::stringstream ssData;
    for (size_t i = 0; i < sizeof(double); ++i) {
        ssData << std::hex << std::setw(2) << std::setfill('0') <<
            static_cast<int>(doubleUnion.bytes[i]);
    }

    StringMsg = idStr + ssData.str();
}

} // namespace udp


int main()
{
    udp::client client_hello;

    client_hello.init();
    client_hello.send_msg();
    //client_hello.recieve();

    return 0;
}
