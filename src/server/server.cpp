#include "server.hpp"
#include <iostream>

namespace cppchat {

Server::Server(boost::asio::io_context& io_context, short port, const std::string& db_path)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      db_(db_path) {
    doAccept();
}

void Server::doAccept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                std::cout << "New connection accepted" << std::endl;
                std::make_shared<Session>(std::move(socket), room_, db_)->start();
            } else {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }
            doAccept();
        });
}

} // namespace cppchat
