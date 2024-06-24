#include "../include/client.hpp"

//#include <json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>

namespace bridge
{

int client::init_port()
{
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    return 0;
}

int client::send_msgs()
{
    //int i = read_json();

    init_vectors();

    for (int id = FIRST_EL_SIMOUT; id <= LAST_EL_SIMOUT; id++) {

        std::string msgStr = parse_msg(id, sim_out[id]);

        std::cout << msgStr << std::endl;

        sendto(
            sockfd, msgStr.c_str(), sizeof(msgStr), 0, (const struct sockaddr *)&servaddr,
            sizeof(servaddr));
    }
    //std::cout << "Message sent to the server." << std::endl;

    close(sockfd);
    return 0;
}


std::string client::parse_msg(int id, double val)
{
    id = id + 10;
    std::stringstream ssID;
    ssID << id;
    std::string idStr = ssID.str();

    union {
        double d;
        uint8_t bytes[sizeof(double)];
    } doubleUnion;

    doubleUnion.d = val;

    std::stringstream ssData;
    for (size_t i = 0; i < sizeof(double); ++i) {
        ssData << std::hex << std::setw(2) << std::setfill('0') <<
            static_cast<int>(doubleUnion.bytes[i]);
    }

    std::string StringMsg = idStr + ssData.str();

    return StringMsg;
}


void client::init_vectors()
{
    for (int i = FIRST_EL_SIMIN; i <= LAST_EL_SIMIN; i++) {
        sim_in[i] = 0.0;
    }

    for (int i = FIRST_EL_SIMOUT; i <= LAST_EL_SIMOUT; i++) {
        sim_out[i] = 0.0;
    }
}


} // namespace bridge


int main()
{
    bridge::client bridge_client;

    int status = 0;

    status = bridge_client.init_port();
    status = bridge_client.send_msgs();

    return 0;
}
