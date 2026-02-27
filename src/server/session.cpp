#include "session.hpp"
#include "database.hpp"
#include <iostream>

namespace cppchat {

void Room::join(SessionPtr participant) {
    participants_.insert(participant);
    for (auto msg : recent_msgs_) {
        participant->deliver(msg);
    }
}

void Room::leave(SessionPtr participant) {
    participants_.erase(participant);
}

void Room::deliver(const Message& msg) {
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs) {
        recent_msgs_.pop_front();
    }

    for (auto participant : participants_) {
        participant->deliver(msg);
    }
}

Session::Session(tcp::socket socket, Room& room, Database& db)
    : ws_(std::move(socket)), room_(room), db_(db) {}

void Session::start() {
    auto self(shared_from_this());
    std::cout << "Starting WebSocket handshake..." << std::endl;
    ws_.async_accept([this, self](beast::error_code ec) {
        if (!ec) {
            std::cout << "WebSocket handshake successful" << std::endl;
            doRead();
        } else {
            std::cerr << "WebSocket handshake failed: " << ec.message() << std::endl;
        }
    });
}

void Session::deliver(const Message& msg) {
    bool write_in_progress = !write_msgs_.empty();
    std::string json_str = msg.toJson().dump();
    
    write_msgs_.push_back(json_str);
    if (!write_in_progress) {
        doWrite();
    }
}

void Session::doRead() {
    auto self(shared_from_this());
    ws_.async_read(buffer_, [this, self](beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (!ec) {
            try {
                std::string msg_str = beast::buffers_to_string(buffer_.data());
                std::cout << "Received message: " << msg_str << std::endl;
                Message msg = Message::fromJson(msg_str);
                handleMessage(msg);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing message: " << e.what() << std::endl;
            }
            buffer_.consume(buffer_.size());
            doRead();
        } else {
            std::cerr << "Read error: " << ec.message() << std::endl;
            room_.leave(self);
        }
    });
}

void Session::doWrite() {
    auto self(shared_from_this());
    ws_.text(true);
    ws_.async_write(boost::asio::buffer(write_msgs_.front()),
        [this, self](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (!ec) {
                write_msgs_.pop_front();
                if (!write_msgs_.empty()) {
                    doWrite();
                }
            } else {
                std::cerr << "Write error: " << ec.message() << std::endl;
                room_.leave(self);
            }
        });
}

void Session::handleMessage(const Message& msg) {
    Message response;
    response.sender = "Server";

    if (msg.type == MessageType::REGISTER_REQ) {
        response.type = MessageType::REGISTER_RESP;
        if (db_.registerUser(msg.sender, msg.password)) {
            response.success = true;
            response.content = "Registration successful.";
        } else {
            response.success = false;
            response.content = "Username already exists.";
        }
        deliver(response);
    } else if (msg.type == MessageType::LOGIN_REQ) {
        response.type = MessageType::LOGIN_RESP;
        if (db_.verifyUser(msg.sender, msg.password)) {
            response.success = true;
            response.content = "Login successful.";
            username_ = msg.sender;
            authenticated_ = true;
            room_.join(shared_from_this());
        } else {
            response.success = false;
            response.content = "Invalid username or password.";
        }
        deliver(response);
    } else if (msg.type == MessageType::CHAT_MSG) {
        if (authenticated_) {
            Message chat_msg = msg;
            chat_msg.sender = username_;
            room_.deliver(chat_msg);
        } else {
            response.type = MessageType::ERROR_MSG;
            response.content = "You must login first.";
            deliver(response);
        }
    }
}

} // namespace cppchat
