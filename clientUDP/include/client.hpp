#pragma once

#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

namespace udp
{

class client
{
private:
    /* variables */
    int sockfd;
    char buffer[BUFFER_SIZE];
    const char * hello = "Hello from client";
    struct sockaddr_in servaddr;

    int n, len;

public:
    /* Functions */
    void create_socket_desc();
    void server_config();
    void send_msg();
    void recieve();

};
} // namespace udp
