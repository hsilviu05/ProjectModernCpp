#pragma once

#include <string>

#ifdef ACCOUNTMANAGER_EXPORTS
#define ACCOUNTMANAGER_API __declspec(dllexport)
#else
#define ACCOUNTMANAGER_API __declspec(dllimport)
#endif

class ACCOUNTMANAGER_API AccountManager {
private:
    std::string username;
    std::string password;
    int points;

public:
    AccountManager();
    AccountManager(const std::string& user, const std::string& pass, int pts = 0);

    void setUsername(const std::string& user);
    void setPassword(const std::string& pass);
    void setPoints(int pts);

    std::string getUsername() const;
    std::string getPassword() const;
    int getPoints() const;

    bool authenticate(const std::string& user, const std::string& pass) const;
    void saveDataToFile(const std::string& filename) const;
    void loadDataFromFile(const std::string& filename);
    
    ~AccountManager();
};
