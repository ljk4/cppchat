#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <deque>
#include <string>
#include "protocol.hpp"

namespace cppchat {

namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = boost::asio::ip::tcp;

class Client {
public:
    Client(boost::asio::io_context& io_context,
           const tcp::resolver::results_type& endpoints);

    void write(const Message& msg);
    void close();

private:
    void doConnect(const tcp::resolver::results_type& endpoints);
    void doRead();
    void doWrite();

    boost::asio::io_context& io_context_;
    websocket::stream<tcp::socket> ws_;
    
    beast::flat_buffer buffer_;
    std::deque<std::string> write_msgs_;
};

} // namespace cppchat
