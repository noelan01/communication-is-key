#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

namespace udp
{
    
class client
{
private:
    /* private variables */
    WSADATA wsaData;
    SOCKET sockfd;
    sockaddr_in servaddr;

public:
    /* public functions */
    int init_client();
    int send_msg();

};

} // namespace udp
