#pragma once

#include <boost/asio.hpp>
#include "session.hpp"
#include "database.hpp"

namespace cppchat {

class Server {
public:
    Server(boost::asio::io_context& io_context, short port, const std::string& db_path);

private:
    void doAccept();

    boost::asio::ip::tcp::acceptor acceptor_;
    Room room_;
    Database db_;
};

} // namespace cppchat
