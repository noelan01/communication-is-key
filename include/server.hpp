#pragma once

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <deque>
#include <cstring>

#define SIMOUT_PORT 49158
#define SIMIN_PORT 49153

#define BUFFER_SIZE 1024

namespace udp
{

// Enum for datatype identification
enum DataType
{
    UNSIGNED_INT,
    DOUBLE,
    VECTOR_INT,
    DEQUE_INT,
    BOOL
};

// Structure to hold the message
struct Message
{
    DataType type;
    unsigned int functionCall; // Identifier for function call
    size_t length; // Length of the data for variable-length data types
    union {
        unsigned int uintValue;
        double doubleValue;
        bool boolValue;
    } data;
    std::vector<int> intVector;
    std::deque<int> intDeque;
};

class server
{
private:
    /* data */
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    int recv_id;
    double recv_data;

    int recvID;
    double recvData;

    int bufferCounter = 0;

public:
    void init_server();
    void recieve_data();
    void parse_msg(std::string msg);
    void parseIncommingMsg(std::string msg);

    void deserializeMessage(const char * buffer, Message & message);
};


} // namespace udp
