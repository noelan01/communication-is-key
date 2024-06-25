#pragma once

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

namespace udp
{
class server
{
private:
    /* data */
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    int recv_id;
    double recv_data;

public:
    void init_server();
    void recieve_data();
    void parse_msg(std::string msg);
};


} // namespace udp
