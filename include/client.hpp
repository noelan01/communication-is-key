#pragma once

#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <deque>
#include <cstring>

#define SIMIN_PORT 49158
#define SIMOUT_PORT 49153

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


class client
{
private:
    /* variables */
    const char * serverIP = "127.0.0.1";

    int sockfd;
    char buffer[BUFFER_SIZE];
    size_t bufferSize;
    const char * hello = "Hello from client";
    char msg[20];
    struct sockaddr_in servaddr;

    int n, len;

    //double sim_in[LAST_EL_SIMIN + 1];
    //double sim_out[LAST_EL_SIMOUT + 1];

    std::string StringMsg;

public:
    /* Functions */
    void init();
    void send_msg();
    void recieve();

    void init_vectors();
    void parse_msg(int id, double data);

    void sendUDP(const char * buffer, size_t length, const char * ip, int port);
    void serialize_message(const Message & message, char * buffer, size_t & bufferSize);

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
} // namespace udp
