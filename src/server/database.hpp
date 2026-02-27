#pragma once

#include <string>
#include <sqlite3.h>
#include <mutex>

namespace cppchat {

class Database {
public:
    Database(const std::string& db_path);
    ~Database();

    bool registerUser(const std::string& username, const std::string& password);
    bool verifyUser(const std::string& username, const std::string& password);

private:
    void initTables();
    sqlite3* db_;
    std::mutex db_mutex_;
};

} // namespace cppchat
