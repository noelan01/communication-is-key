#include "../include/client.hpp"

namespace bridge
{

} // namespace bridge


int main()
{
    try {
        boost::asio::io_context io_context;

        boost::asio::ip::udp::resolver resolver(io_context);
        boost::asio::ip::udp::resolver::results_type endpoints = resolver.resolve(
            boost::asio::ip::udp::v4(), "localhost", std::to_string(PORT));

        boost::asio::ip::udp::socket socket(io_context);
        socket.open(boost::asio::ip::udp::v4());

        std::string message = "Hello from the client!";
        socket.send_to(boost::asio::buffer(message), *endpoints.begin());

        std::cout << "Message sent to the server." << std::endl;
    } catch (std::exception & e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
