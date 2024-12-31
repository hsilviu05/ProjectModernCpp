
#include "pch.h"
#include "AccountManager.h"
#include <fstream>
#include <iostream>

AccountManager::AccountManager()
    : username(""), password(""), points(0), score(0), isSpeedBoost(false), isSpeedUpgrade(false) {
}
AccountManager::AccountManager(const std::string& user, const std::string& pass, int pts, int scr, bool iSB , bool iSU)
    : username(user), password(pass), points(pts), score(scr), isSpeedBoost(iSB), isSpeedUpgrade(iSU) {
}

void AccountManager::setUsername(const std::string& user) { username = user; }
void AccountManager::setPassword(const std::string& pass) { password = pass; }
void AccountManager::setPoints(int pts) { points = pts; }

std::string AccountManager::getUsername() const { return username; }
std::string AccountManager::getPassword() const { return password; }
int AccountManager::getPoints() const { return points; }
int AccountManager::getScore() const { return score; }

bool AccountManager::GetSpeedBoost() const { return isSpeedBoost; }
bool AccountManager::GetSpeedUpgrade() const { return isSpeedUpgrade; }

bool AccountManager::authenticate(const std::string& user, const std::string& pass) const {
    return username == user && password == pass;
}

void AccountManager::saveDataToDatabase(const std::string& dbFile) const {
    sqlite3* db;
    char* errMsg = nullptr;

    if (sqlite3_open(dbFile.c_str(), &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS Account (
            Username TEXT PRIMARY KEY,
            Password TEXT,
            Points INTEGER,
            Score INTEGER,
            isSpeedBoost BOOL,
            isSpeedUpgrade BOOL
        );
    )";

    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return;
    }

    const char* insertSQL = R"(
        INSERT OR REPLACE INTO Account (Username, Password, Points, Score, isSpeedBoost, isSpeedUpgrade)
        VALUES (?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, points);
    sqlite3_bind_int(stmt, 4, score);
    sqlite3_bind_int(stmt, 5, isSpeedBoost);
    sqlite3_bind_int(stmt, 6, isSpeedUpgrade);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void AccountManager::loadDataFromDatabase(const std::string& dbFile, const std::string& user) {
    sqlite3* db;

    if (sqlite3_open(dbFile.c_str(), &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* querySQL = R"(
        SELECT Username, Password, Points, Score, isSpeedBoost, isSpeedUpgrade
        FROM Account
        WHERE Username = ?;
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, querySQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, user.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        points = sqlite3_column_int(stmt, 2);
        score = sqlite3_column_int(stmt, 3);
        isSpeedBoost = sqlite3_column_int(stmt, 4);
        isSpeedUpgrade = sqlite3_column_int(stmt, 5);
    }
    else {
        std::cerr << "No data found for user: " << user << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

AccountManager::~AccountManager()
{
}
