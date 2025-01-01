
#include "pch.h"
#include "AccountManager.h"
#include <fstream>
#include <iostream>

AccountManager::AccountManager()
    : username(""), password(""), points(0), score(0), isSpeedBoost(false), isSpeedUpgrade(false) {
}
AccountManager::AccountManager(const std::string& user, const std::string& pass, int pts, int scr, bool iSB, bool iSU)
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

inline auto AccountManager::initStorage(const std::string& dbFile) const
{
    using namespace sqlite_orm;
    return make_storage(
        dbFile,
        make_table("Account",
            make_column("Username", &AccountManager::username, primary_key()),
            make_column("Password", &AccountManager::password),
            make_column("Points", &AccountManager::points),
            make_column("Score", &AccountManager::score),
            make_column("isSpeedBoost", &AccountManager::isSpeedBoost),
            make_column("isSpeedUpgrade", &AccountManager::isSpeedUpgrade))
    );
}

void AccountManager::saveDataToDatabase(const std::string& dbFile) const
{
    auto storage = initStorage(dbFile);
    storage.sync_schema();
    storage.replace(*this);
}


void AccountManager::loadDataFromDatabase(const std::string& dbFile, const std::string& user) {
    auto storage = initStorage(dbFile);
    storage.sync_schema();

    auto account = storage.get_pointer<AccountManager>(user);
    if (account) {
        *this = *account;
    }
    else {
        std::cerr << "No data found for user: " << user << std::endl;
    }
}

void AccountManager::signUp(const std::string& dbFile, const std::string& user, const std::string& pass)
{
    auto storage = initStorage(dbFile);
    storage.sync_schema();

    auto existingAccount = storage.get_pointer<AccountManager>(user);

    if(existingAccount){
        std::cerr << "Account already exists.Please chose another one. "<< std::endl;
        return;
    }
    AccountManager newAccount(user, pass, 0, 0, false, false);
    storage.replace(newAccount);
    std::cout << " Account created succesfully!" << std::endl;
}

void AccountManager::loginForm(const std::string& dbFile)
{
    AccountManager account;
    while (true) {
        std::cout << "\n....LoginForm....\n";
        std::cout << "1.Login\n";
        std::cout << "2.Sign up\n";
        std::cout << "3.Exit\n";
        std::cout << "\nChose your option : \n";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter Username: ";
            std::cin >> username;
            std::cout << "Enter Password: ";
            std::cin >> password;

            account.loadDataFromDatabase(dbFile, username);
            if (account.authenticate(username, password)) {
                std::cout << "Login successful! Welcome, " << username << "!\n";
            }
            else {
                std::cout << "Invalid credentials. Please try again.\n";
            }
            break;

        case 2:

            std::cout << "Choose a Username: ";
            std::cin >> username;
            std::cout << "Choose a Password: ";
            std::cin >> password;

            account.signUp(dbFile, username, password);
            break;

        case 3:
            std::cout << "Exiting...\n";
            return;

        default:
            std::cout << "Invalid option. Please try again.\n";

        }

    }
}

AccountManager::~AccountManager()
{
}
