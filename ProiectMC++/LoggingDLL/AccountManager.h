#pragma once

#include <string>
#include <sqlite3.h>

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

class DLL_EXPORT AccountManager {
private:
    std::string username;
    std::string password;
    int points;
    int score;
    bool isSpeedBoost;
    bool isSpeedUpgrade;

public:
    AccountManager();
    AccountManager(const std::string& user, const std::string& pass, int pts, int scr, bool iSB, bool iSU);

    void setUsername(const std::string& user);
    void setPassword(const std::string& pass);
    void setPoints(int pts);

    std::string getUsername() const;
    std::string getPassword() const;
    int getPoints() const;
    int getScore() const;

    bool GetSpeedBoost() const;
    bool GetSpeedUpgrade() const;

    bool authenticate(const std::string& user, const std::string& pass) const;
    void saveDataToDatabase(const std::string& dbFile) const;
    void loadDataFromDatabase(const std::string& dbFile, const std::string& user);

    ~AccountManager();
};
