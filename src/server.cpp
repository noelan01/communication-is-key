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
    servaddr.sin_port = htons(SIMIN_PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cout << "bind failed" << std::endl;
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void server::parseIncommingMsg(std::string msg)
{
    std::string idStr = msg.substr(0, 2);
    msg.erase(0, 2);

    std::stringstream ss;
    ss << idStr;
    ss >> recvID;
    recvID = recvID - 10;

    union {
        double d;
        uint8_t bytes[sizeof(double)];
    } doubleUnion;

    // Parse the hex string back into bytes
    for (size_t i = 0; i < sizeof(double); ++i) {
        std::string byteString = msg.substr(i * 2, 2);
        doubleUnion.bytes[i] = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
    }

    recvData = doubleUnion.d;
}

void server::recieve_data()
{
    while (true) {
        socklen_t clientAddrLen = sizeof(cliaddr);
        int bytesRead = recvfrom(
            sockfd, (char *)buffer, BUFFER_SIZE, MSG_DONTWAIT,
            (struct sockaddr *)&cliaddr, &clientAddrLen);

        // Check if buffer is empty
        if (bytesRead == -1) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                // No data available, buffer is empty
                continue;
            } else {
                std::cerr << "recvfrom error" << std::endl;
                break;
            }
        }

        Message receivedMessage;
        deserializeMessage(buffer, receivedMessage);

        // Process received message based on type and function call
        switch (receivedMessage.type) {
          case UNSIGNED_INT:
              std::cout << "Received UNSIGNED_INT: " << receivedMessage.data.uintValue << std::endl;
              break;
          case DOUBLE:
              std::cout << "Received DOUBLE: " << receivedMessage.data.doubleValue << std::endl;
              break;
          case BOOL:
              std::cout << "Received BOOL: " << receivedMessage.data.boolValue << std::endl;
              break;
          case VECTOR_INT:
              std::cout << "Received VECTOR_INT: ";
              for (int value : receivedMessage.intVector) {
                  std::cout << value << " ";
              }
              std::cout << std::endl;
              break;
          case DEQUE_INT:
              std::cout << "Received DEQUE_INT: ";
              for (int value : receivedMessage.intDeque) {
                  std::cout << value << " ";
              }
              std::cout << std::endl;
              break;
        }

    }

    close(sockfd);
}

void server::deserializeMessage(const char * buffer, Message & message)
{
    size_t offset = 0;

    std::memcpy(&message.type, buffer + offset, sizeof(message.type));
    offset += sizeof(message.type);

    std::memcpy(&message.functionCall, buffer + offset, sizeof(message.functionCall));
    offset += sizeof(message.functionCall);

    std::memcpy(&message.length, buffer + offset, sizeof(message.length));
    offset += sizeof(message.length);

    switch (message.type) {
      case UNSIGNED_INT:
        {
            unsigned int networkInt;
            std::memcpy(&networkInt, buffer + offset, sizeof(networkInt));
            message.data.uintValue = ntohl(networkInt);
            offset += sizeof(networkInt);
        }
        break;
      case DOUBLE:
          std::memcpy(&message.data.doubleValue, buffer + offset, sizeof(message.data.doubleValue));
          offset += sizeof(message.data.doubleValue);
          break;
      case BOOL:
          std::memcpy(&message.data.boolValue, buffer + offset, sizeof(message.data.boolValue));
          offset += sizeof(message.data.boolValue);
          break;
      case VECTOR_INT:
          message.intVector.resize(message.length);
          for (size_t i = 0; i < message.length; ++i) {
              int networkInt;
              std::memcpy(&networkInt, buffer + offset, sizeof(networkInt));
              message.intVector[i] = ntohl(networkInt);
              offset += sizeof(networkInt);
          }
          break;
      case DEQUE_INT:
          message.intDeque.resize(message.length);
          for (size_t i = 0; i < message.length; ++i) {
              int networkInt;
              std::memcpy(&networkInt, buffer + offset, sizeof(networkInt));
              message.intDeque[i] = ntohl(networkInt);
              offset += sizeof(networkInt);
          }
          break;
    }
}

/*
void server::parse_msg(std::string msg)
{
    std::string idStr = msg.substr(0, 2);
    msg.erase(0, 2);

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
*/

} // namespace udp


int main()
{
    udp::server server_1;

    server_1.init_server();
    server_1.recieve_data();

    return 0;
}
