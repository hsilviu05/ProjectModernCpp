#pragma once

#include <string>
#include <sqlite_orm/sqlite_orm.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <chrono>
//pt operatia de hassing a parolei avem urmatoarele biblioteci
#include <iomanip>
#include <sstream>
//#include <nlohmann/json.hpp>
#include <crow.h>

using namespace sqlite_orm;

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

class DLL_EXPORT AccountManager {
private:
    std::string username;
    std::string password;
    size_t health;
    uint32_t fireRate;
    uint16_t points;
    uint16_t score; 
    bool isFireRateUpgrade; 
    bool isSpeedUpgrade; 
    double bulletSpeed; 

public:
    AccountManager();
    AccountManager(const std::string& user, const std::string& pass, size_t heal, uint32_t fRate, uint16_t pts, uint16_t scr, bool iFRU, bool iSU, double bSpeed);

    void SetUsername(const std::string& user);
    void SetPassword(const std::string& pass);
    void SetHealth(size_t& heal);
    void SetFireRate(uint32_t& fireRate);
    void SetPoints(uint16_t& pts);
    void SetBulletSpeed(double& bSpeed);

    std::string GetUsername() const;
    std::string GetPassword() const;
    uint16_t GetPoints() const;
    uint16_t GetScore() const;
    uint32_t GetFireRate() const;

    bool GetSpeedBoost() const;
    bool GetSpeedUpgrade() const;

    bool Authenticate(const std::string& user, const std::string& pass) const;

    inline auto InItStorage(const std::string& dbFile) const;
    void SaveDataToDatabase(const std::string& dbFile) const;
    void LoadDataFromDatabase(const std::string& dbFile, const std::string& user);

    void SignUp(const std::string& dbFile, const std::string& user, const std::string& pass);

    void LoginForm(const std::string& dbFile);

    bool IsValidPassword(const std::string& pass) const;

    std::string HashPassword(const std::string& pass) const;

    crow::json::wvalue To_json() const;

    ~AccountManager();
};
