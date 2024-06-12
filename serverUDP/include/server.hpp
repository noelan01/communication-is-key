#pragma once

#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

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

public:
    void init_server();
    void recieve_data();
};


} // namespace udp
