#include "client.hpp"
#include <iostream>

namespace cppchat {

Client::Client(boost::asio::io_context& io_context,
               const tcp::resolver::results_type& endpoints)
    : io_context_(io_context), ws_(io_context) {
    doConnect(endpoints);
}

void Client::write(const Message& msg) {
    std::string json_str = msg.toJson().dump();

    boost::asio::post(io_context_,
        [this, json_str]() {
            bool write_in_progress = !write_msgs_.empty();
            write_msgs_.push_back(json_str);
            if (!write_in_progress) {
                doWrite();
            }
        });
}

void Client::close() {
    boost::asio::post(io_context_, [this]() { 
        ws_.async_close(websocket::close_code::normal, [this](beast::error_code ec) {
            // closed
        });
    });
}

void Client::doConnect(const tcp::resolver::results_type& endpoints) {
    boost::asio::async_connect(ws_.next_layer(), endpoints,
        [this](beast::error_code ec, tcp::endpoint ep) {
            if (!ec) {
                std::string host = ep.address().to_string() + ":" + std::to_string(ep.port());
                ws_.async_handshake(host, "/", [this](beast::error_code ec) {
                    if (!ec) {
                        doRead();
                    } else {
                        std::cerr << "Handshake failed: " << ec.message() << std::endl;
                    }
                });
            } else {
                std::cerr << "Connect failed: " << ec.message() << std::endl;
            }
        });
}

void Client::doRead() {
    ws_.async_read(buffer_,
        [this](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (!ec) {
                try {
                    std::string msg_str = beast::buffers_to_string(buffer_.data());
                    Message msg = Message::fromJson(msg_str);
                    if (msg.type == MessageType::CHAT_MSG) {
                        std::cout << msg.sender << ": " << msg.content << std::endl;
                    } else if (msg.type == MessageType::REGISTER_RESP || msg.type == MessageType::LOGIN_RESP) {
                        std::cout << "Server: " << msg.content << std::endl;
                    } else if (msg.type == MessageType::ERROR_MSG) {
                        std::cerr << "Error: " << msg.content << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing message: " << e.what() << std::endl;
                }
                buffer_.consume(buffer_.size());
                doRead();
            } else {
                ws_.next_layer().close();
            }
        });
}

void Client::doWrite() {
    ws_.text(true);
    ws_.async_write(boost::asio::buffer(write_msgs_.front()),
        [this](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (!ec) {
                write_msgs_.pop_front();
                if (!write_msgs_.empty()) {
                    doWrite();
                }
            } else {
                ws_.next_layer().close();
            }
        });
}

} // namespace cppchat
