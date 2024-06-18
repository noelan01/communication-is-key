#include "../include/server.hpp"

namespace bridge
{

} // namespace bridge


int main()
{
    try {
        boost::asio::io_context io_context;

        boost::asio::ip::udp::socket socket(io_context,
          boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), PORT));

        std::cout << "Server is running on port " << PORT << std::endl;

        while (true) {
            char data[1024];
            boost::asio::ip::udp::endpoint sender_endpoint;
            boost::system::error_code error;

            size_t length = socket.receive_from(
                boost::asio::buffer(
                    data,
                    1024), sender_endpoint, 0,
                error);

            if (error && error != boost::asio::error::message_size) {
                throw boost::system::system_error(error);
            }

            std::cout << "Received: " << std::string(data, length) << std::endl;
        }
    } catch (std::exception & e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
