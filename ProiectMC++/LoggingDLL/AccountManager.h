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
    uint16_t fireRate;
    uint16_t points;
    uint16_t score; 
    bool isSpeedUpgrade;
    uint16_t FireRateUpgrade;

public:
    AccountManager();
    AccountManager(const std::string& user, const std::string& pass, uint16_t fRate, uint16_t pts, uint16_t scr, uint16_t iFRU, bool sU);

    void SetUsername(const std::string& user);
    void SetPassword(const std::string& pass);
    void SetFireRate(uint16_t fireRate);
    void SetPoints(uint16_t pts);
    void SetScore(uint16_t scr);
    void SetBulletSpeedUpgrade(bool upgr);
    void SetFireRateUpgrades(uint16_t fru);

    std::string GetUsername() const;
    std::string GetPassword() const;
    uint16_t GetPoints() const;
    uint16_t GetScore() const;
    uint16_t GetFireRate() const;
    bool GetIsSpeedUpgrade() const;
    
    uint16_t GetFireRateUpgrades() const;

    bool Authenticate(const std::string& user, const std::string& pass) const;

    inline auto InItStorage(const std::string& dbFile) const;
    void SaveDataToDatabase(const std::string& dbFile) const;
    void LoadDataFromDatabase(const std::string& dbFile, const std::string& user);

    void SignUp(const std::string& dbFile, const std::string& user, const std::string& pass);

    void LoginForm(const std::string& dbFile);

    bool IsValidPassword(const std::string& pass) const;

    std::string HashPassword(const std::string& pass) const;

    ~AccountManager();
};
