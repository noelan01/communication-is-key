#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <iomanip>

#include "../include/client.hpp"

#define SIMIN_PORT 49158
#define SIMOUT_PORT 49153

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
    servaddr.sin_port = htons(SIMOUT_PORT);
    servaddr.sin_addr.s_addr = inet_addr(serverIP);  // server IP (localhost)
}


void client::send_msg()
{
    //init_vectors();

    Message message;
    message.type = VECTOR_INT;
    message.functionCall = 1; // Example function call identifier
    message.intVector = {1, 2, 3, 4, 5};
    message.length = message.intVector.size();

    serialize_message(message, buffer, bufferSize);
    sendUDP(buffer, bufferSize, serverIP, SIMOUT_PORT);

    message.type = DOUBLE;
    message.functionCall = 1; // Example function call identifier
    message.data.doubleValue = 8.8888;
    message.length = 0;

    serialize_message(message, buffer, bufferSize);
    sendUDP(buffer, bufferSize, serverIP, SIMOUT_PORT);


    /*if (sendto(
          sockfd, &msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *)&servaddr,
          sizeof(servaddr)) < 0)
    {
        std::cout << "sendto error" << std::endl;
        perror("sendto error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }*/


    close(sockfd);
}


void client::sendUDP(const char * buffer, size_t length, const char * ip, int port)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);

    sendto(sockfd, buffer, length, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    close(sockfd);
}

/*
void client::init_vectors()
{
    for (int i = FIRST_EL_SIMIN; i <= LAST_EL_SIMIN; i++) {
        sim_in[i] = 5.0;
    }

    for (int i = FIRST_EL_SIMOUT; i <= LAST_EL_SIMOUT; i++) {
        sim_out[i] = 10.55;
    }
}
*/
int hton_int(int value)
{
    return htonl(value);
}

int ntoh_int(int value)
{
    return ntohl(value);
}

void client::serialize_message(const Message & message, char * buffer, size_t & bufferSize)
{
    size_t offset = 0;

    std::memcpy(buffer + offset, &message.type, sizeof(message.type));
    offset += sizeof(message.type);

    std::memcpy(buffer + offset, &message.functionCall, sizeof(message.functionCall));
    offset += sizeof(message.functionCall);

    std::memcpy(buffer + offset, &message.length, sizeof(message.length));
    offset += sizeof(message.length);

    switch (message.type) {
      case UNSIGNED_INT:
        {
            unsigned int networkInt = htonl(message.data.uintValue);
            std::memcpy(buffer + offset, &networkInt, sizeof(networkInt));
            offset += sizeof(networkInt);
        }
        break;
      case DOUBLE:
          std::memcpy(buffer + offset, &message.data.doubleValue, sizeof(message.data.doubleValue));
          offset += sizeof(message.data.doubleValue);
          break;
      case BOOL:
          std::memcpy(buffer + offset, &message.data.boolValue, sizeof(message.data.boolValue));
          offset += sizeof(message.data.boolValue);
          break;
      case VECTOR_INT:
          for (size_t i = 0; i < message.length; ++i) {
              int networkInt = htonl(message.intVector[i]);
              std::memcpy(buffer + offset, &networkInt, sizeof(networkInt));
              offset += sizeof(networkInt);
          }
          break;
      case DEQUE_INT:
          for (size_t i = 0; i < message.length; ++i) {
              int networkInt = htonl(message.intDeque[i]);
              std::memcpy(buffer + offset, &networkInt, sizeof(networkInt));
              offset += sizeof(networkInt);
          }
          break;
    }

    bufferSize = offset;
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

/*
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
*/

} // namespace udp


int main()
{
    udp::client client_hello;

    client_hello.init();
    client_hello.send_msg();
    //client_hello.recieve();

    return 0;
}
