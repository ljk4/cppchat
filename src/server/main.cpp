#include <iostream>
#include <boost/asio.hpp>
#include "server.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: cppchat_server <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        cppchat::Server server(io_context, std::atoi(argv[1]), "chat.db");
        
        std::cout << "Server started on port " << argv[1] << std::endl;
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
