#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <memory>
#include <set>
#include <string>
#include <deque>
#include "protocol.hpp"

namespace cppchat {

namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = boost::asio::ip::tcp;

class Session;
using SessionPtr = std::shared_ptr<Session>;

class Room {
public:
    void join(SessionPtr participant);
    void leave(SessionPtr participant);
    void deliver(const Message& msg);

private:
    std::set<SessionPtr> participants_;
    enum { max_recent_msgs = 100 };
    std::deque<Message> recent_msgs_;
};

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Room& room, class Database& db);
    void start();
    void deliver(const Message& msg);

private:
    void doRead();
    void doWrite();
    void handleMessage(const Message& msg);

    websocket::stream<tcp::socket> ws_;
    Room& room_;
    class Database& db_;
    
    beast::flat_buffer buffer_;
    std::deque<std::string> write_msgs_;
    std::string username_;
    bool authenticated_ = false;
};

} // namespace cppchat
