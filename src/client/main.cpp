#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "client.hpp"

using namespace cppchat;

void printMenu() {
    std::cout << "\nWelcome to CppChat!\n"
              << "1. Register\n"
              << "2. Login\n"
              << "3. Exit\n"
              << "Please choose an option: ";
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: cppchat_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);

        Client client(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        bool authenticated = false;
        std::string username;

        while (!authenticated) {
            printMenu();
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "1" || choice == "2") {
                std::cout << "Username: ";
                std::string user;
                std::getline(std::cin, user);

                std::cout << "Password: ";
                std::string pass;
                std::getline(std::cin, pass);

                Message msg;
                msg.type = (choice == "1") ? MessageType::REGISTER_REQ : MessageType::LOGIN_REQ;
                msg.sender = user;
                msg.password = pass;

                client.write(msg);
                
                // Simple wait for response (in a real app, use condition variable or async handling)
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                
                if (choice == "2") {
                    // Assume login success for simplicity in this loop, 
                    // actual validation is printed by async read
                    authenticated = true;
                    username = user;
                }
            } else if (choice == "3") {
                client.close();
                t.join();
                return 0;
            } else {
                std::cout << "Invalid choice.\n";
            }
        }

        std::cout << "Enter messages (type '/quit' to exit):\n";
        char line[Message::max_body_length + 1];
        while (std::cin.getline(line, Message::max_body_length + 1)) {
            std::string input(line);
            if (input == "/quit") {
                break;
            }
            
            Message msg;
            msg.type = MessageType::CHAT_MSG;
            msg.sender = username;
            msg.content = input;
            client.write(msg);
        }

        client.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
