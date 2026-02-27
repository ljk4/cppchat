#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace cppchat {

enum class MessageType {
    REGISTER_REQ,
    REGISTER_RESP,
    LOGIN_REQ,
    LOGIN_RESP,
    CHAT_MSG,
    ERROR_MSG
};

struct Message {
    MessageType type;
    std::string sender;
    std::string content;
    std::string password; // Used for auth
    bool success = false; // Used for responses

    static constexpr size_t max_body_length = 512;

    nlohmann::json toJson() const {
        nlohmann::json j;
        j["type"] = static_cast<int>(type);
        j["sender"] = sender;
        j["content"] = content;
        if (!password.empty()) j["password"] = password;
        j["success"] = success;
        return j;
    }

    static Message fromJson(const std::string& json_str) {
        auto j = nlohmann::json::parse(json_str);
        Message msg;
        msg.type = static_cast<MessageType>(j["type"].get<int>());
        msg.sender = j.value("sender", "");
        msg.content = j.value("content", "");
        msg.password = j.value("password", "");
        msg.success = j.value("success", false);
        return msg;
    }
};

} // namespace cppchat
