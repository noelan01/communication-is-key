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

//class simIn;
//class simOut;

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

enum simIn_id
{
    TFL,
    TFR,
    TRL,
    TRR,

    FIRST_EL_SIMIN = TFL,
    LAST_EL_SIMIN = TRR
};


class client
{
private:
    /* data */
    int sockfd;
    char buffer[BUFSIZE];
    struct sockaddr_in servaddr;

public:
    client(/* args */);
    ~client();

    int init_port();
    int send_msgs();
    int read_json();
    void init_vectors();
    std::string parse_msg(int id, double val);

    double sim_out[LAST_EL_SIMOUT];
    double sim_in[LAST_EL_SIMIN];
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
        APPS,         // driver controls
        BPPS,
        BPS,
        SWA,

        NFL,         // wheelspeeds
        NFR,
        NRL,
        NRR,

        VX,         // EKF
        VY,

        IRESULT,         // Battery data
        PRESULT,
        URESULT,
        SOC,

        FNSTATE,         // states and flags
        RNSTATE,
        SWAFLAG,

        LINACC_X,         // imu
        LINACC_Y,
        LINACC_Z,
        ANGVEL_X,
        ANGVEL_Y,
        ANGVEL_Z,

        LAUNCHCTRREQ,         // other
        TVLVLUP,
        LAPNUM
    };

};

} // namespace bridge
