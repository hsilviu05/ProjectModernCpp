
#include "pch.h"
#include "AccountManager.h"
#include <fstream>a
#include <iostream>

AccountManager::AccountManager() : username(""), password(""), points(0) {}

AccountManager::AccountManager(const std::string& user, const std::string& pass, int pts)
    : username(user), password(pass), points(pts) {
}

void AccountManager::setUsername(const std::string& user) { username = user; }
void AccountManager::setPassword(const std::string& pass) { password = pass; }
void AccountManager::setPoints(int pts) { points = pts; }

std::string AccountManager::getUsername() const { return username; }
std::string AccountManager::getPassword() const { return password; }
int AccountManager::getPoints() const { return points; }

bool AccountManager::authenticate(const std::string& user, const std::string& pass) const {
    return username == user && password == pass;
}

void AccountManager::saveDataToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Unable to open file for saving\n";
        return;
    }
    outFile << username << "\n" << password << "\n" << points << "\n";
    outFile.close();
}

void AccountManager::loadDataFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Unable to open file for loading\n";
        return;
    }
    std::getline(inFile, username);
    std::getline(inFile, password);
    inFile >> points;
    inFile.close();
}

AccountManager::~AccountManager()
{
}
