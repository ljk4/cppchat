#include <gtest/gtest.h>
#include "protocol.hpp"

using namespace cppchat;

TEST(ProtocolTest, ToJson) {
    Message msg;
    msg.type = MessageType::CHAT_MSG;
    msg.sender = "alice";
    msg.content = "hello world";
    msg.password = "secret";
    msg.success = true;

    nlohmann::json j = msg.toJson();

    EXPECT_EQ(j["type"], static_cast<int>(MessageType::CHAT_MSG));
    EXPECT_EQ(j["sender"], "alice");
    EXPECT_EQ(j["content"], "hello world");
    EXPECT_EQ(j["password"], "secret");
    EXPECT_EQ(j["success"], true);
}

TEST(ProtocolTest, FromJson) {
    nlohmann::json j;
    j["type"] = static_cast<int>(MessageType::LOGIN_REQ);
    j["sender"] = "bob";
    j["content"] = "";
    j["password"] = "123456";
    j["success"] = false;

    Message msg = Message::fromJson(j.dump());

    EXPECT_EQ(msg.type, MessageType::LOGIN_REQ);
    EXPECT_EQ(msg.sender, "bob");
    EXPECT_EQ(msg.content, "");
    EXPECT_EQ(msg.password, "123456");
    EXPECT_EQ(msg.success, false);
}

TEST(ProtocolTest, FromJsonMissingFields) {
    nlohmann::json j;
    j["type"] = static_cast<int>(MessageType::REGISTER_REQ);
    // Missing sender, content, password, success

    Message msg = Message::fromJson(j.dump());

    EXPECT_EQ(msg.type, MessageType::REGISTER_REQ);
    EXPECT_EQ(msg.sender, "");
    EXPECT_EQ(msg.content, "");
    EXPECT_EQ(msg.password, "");
    EXPECT_EQ(msg.success, false);
}
