#include <gtest/gtest.h>
#include "database.hpp"
#include <cstdio>

using namespace cppchat;

class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        db_path = "test_chat.db";
        std::remove(db_path.c_str());
        db = new Database(db_path);
    }

    void TearDown() override {
        delete db;
        std::remove(db_path.c_str());
    }

    Database* db;
    std::string db_path;
};

TEST_F(DatabaseTest, RegisterUserSuccess) {
    EXPECT_TRUE(db->registerUser("testuser", "password123"));
}

TEST_F(DatabaseTest, RegisterUserDuplicate) {
    EXPECT_TRUE(db->registerUser("testuser", "password123"));
    EXPECT_FALSE(db->registerUser("testuser", "password456"));
}

TEST_F(DatabaseTest, VerifyUserSuccess) {
    db->registerUser("testuser", "password123");
    EXPECT_TRUE(db->verifyUser("testuser", "password123"));
}

TEST_F(DatabaseTest, VerifyUserFailure) {
    db->registerUser("testuser", "password123");
    EXPECT_FALSE(db->verifyUser("testuser", "wrongpassword"));
    EXPECT_FALSE(db->verifyUser("nonexistent", "password123"));
}
