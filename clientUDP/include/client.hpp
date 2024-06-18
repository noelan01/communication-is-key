#pragma once

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <bitset>
#include <iomanip>

#define PORT 8080
#define BUFSIZE 1024


namespace bridge
{

class client
{
private:
    /* data */
    int sockfd;
    char buffer[BUFSIZE];
    struct sockaddr_in servaddr;

    //simOut sim_out;
    //simIn sim_in;

    double msg = 55.4345;


public:
    client(/* args */);
    ~client();

    int init_port();
    int send_msg();
    std::string double2hex(double val);

};

client::client(/* args */)
{
}

client::~client()
{
}

struct simIn
{
    float tThrReq;

    double * tFl;
    double * tFr;
    double * tRl;
    double * tRr;

};


struct simOut
{
    double * apps;         // driver controls
    double * bpps;
    double * bps;
    double * swa;

    double * nFl;         // wheelspeeds
    double * nFr;
    double * nRl;
    double * nRr;

    double * vx;         // EKF
    double * vy;

    double * iResult;         // Battery data
    double * pResult;
    double * uResult;
    double * soc;

    double * fnState;         // states and flags
    double * rnState;
    double * swaFlag;

    double * linAcc_x;         // imu
    double * linAcc_y;
    double * linAcc_z;
    double * angVel_x;
    double * angVel_y;
    double * angVel_z;

    double * launchCtrlReq;         // other
    double * tvLvlUp;
    double * lapNum;
};

} // namespace bridge
