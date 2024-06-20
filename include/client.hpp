#pragma once

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <bitset>
#include <iomanip>
#include <vector>

#define PORT 8080
#define BUFSIZE 1024


namespace bridge
{

class simIn;
class simOut;

class client
{
private:
    /* data */
    int sockfd;
    char buffer[BUFSIZE];
    struct sockaddr_in servaddr;


    double msg = 123.456;

public:
    client(/* args */);
    ~client();

    int init_port();
    int send_msgs();
    int read_json();
    std::string parse_msg(double val);

    std::vector<std::vector<double>> sim_out;
    std::vector<std::vector<double>> sim_in;


    //simOut * sim_out;
    //simIn * sim_in;

};

client::client(/* args */)
{
}

client::~client()
{
}

struct simIn
{
    struct val
    {
        double * tFl;
        double * tFr;
        double * tRl;
        double * tRr;
    };

    enum id
    {
        tFl,
        tFr,
        tRl,
        tRr
    };
};


struct simOut
{
    struct val
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

    enum id
    {
        apps,         // driver controls
        bpps,
        bps,
        swa,

        nFl,         // wheelspeeds
        nFr,
        nRl,
        nRr,

        vx,         // EKF
        vy,

        iResult,         // Battery data
        pResult,
        uResult,
        soc,

        fnState,         // states and flags
        rnState,
        swaFlag,

        linAcc_x,         // imu
        linAcc_y,
        linAcc_z,
        angVel_x,
        angVel_y,
        angVel_z,

        launchCtrlReq,         // other
        tvLvlUp,
        lapNum
    };

};

} // namespace bridge
