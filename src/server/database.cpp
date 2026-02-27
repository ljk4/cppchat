#include "database.hpp"
#include <iostream>
#include <stdexcept>

namespace cppchat {

Database::Database(const std::string& db_path) {
    if (sqlite3_open(db_path.c_str(), &db_) != SQLITE_OK) {
        throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(db_)));
    }
    initTables();
}

Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
    }
}

void Database::initTables() {
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "username TEXT UNIQUE NOT NULL,"
                      "password TEXT NOT NULL);";
    char* err_msg = nullptr;
    if (sqlite3_exec(db_, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::string err = err_msg;
        sqlite3_free(err_msg);
        throw std::runtime_error("Failed to create tables: " + err);
    }
}

bool Database::registerUser(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(db_mutex_);
    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool Database::verifyUser(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(db_mutex_);
    const char* sql = "SELECT password FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string stored_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        success = (stored_password == password);
    }

    sqlite3_finalize(stmt);
    return success;
}

} // namespace cppchat
