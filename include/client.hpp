#pragma once

#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

namespace udp
{
    enum simIn_id
{
    TFL,
    TFR,
    TRL,
    TRR,

    FIRST_EL_SIMIN = TFL,
    LAST_EL_SIMIN = TRR
};

enum simOut_id
{
    APPS,             // driver controls
    BPPS,
    BPS,
    SWA,

    NFL,             // wheelspeeds
    NFR,
    NRL,
    NRR,

    VX,             // EKF
    VY,

    IRESULT,             // Battery data
    PRESULT,
    URESULT,
    SOC,

    FNSTATE,             // states and flags
    RNSTATE,
    SWAFLAG,

    LINACC_X,             // imu
    LINACC_Y,
    LINACC_Z,
    ANGVEL_X,
    ANGVEL_Y,
    ANGVEL_Z,

    LAUNCHCTRREQ,             // other
    TVLVLUP,
    LAPNUM,

    FIRST_EL_SIMOUT = APPS,
    LAST_EL_SIMOUT = LAPNUM
};

class client
{
private:
    /* variables */
    const char * serverIP = "127.0.0.1";

    int sockfd;
    char buffer[BUFFER_SIZE];
    const char * hello = "Hello from client";
    char msg[20];
    struct sockaddr_in servaddr;

    int n, len;

    double sim_in[LAST_EL_SIMIN + 1];
    double sim_out[LAST_EL_SIMOUT + 1];

    std::string StringMsg;

public:
    /* Functions */
    void init();
    void send_msg();
    void recieve();

    void init_vectors();
    void parse_msg(int id, double data);

};
} // namespace udp
