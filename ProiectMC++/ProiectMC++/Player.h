#pragma once
#include "Direction.h"
#include "Bullet.h"
#include "GameObject.h"
#include "GameSettings.h"
#include <chrono>
#include <memory>


#include <vector>

class Player:public GameObject {
public:
    
    Player(const std::string& username, std::chrono::milliseconds& fireRate, uint8_t& fireRateUpgrades, double& bulletSpeed, bool& bulletSpeedUpgraded);
    Player();
    void move(const char& key);
    void TakeDamage();
    bool isAlive() const;
    void respawn(const std::pair<size_t, size_t>& initialPosition);
    size_t getHealth() const;
    bool isEliminated() const;
    Direction getDirection() const;
    void setDirection(Direction player_direction);

    void AddWinBonus();
    bool UpgradeWeapon();
    void AddScore();
    uint16_t GetScore() const;
    bool UpgradeBulletSpeed();
    uint8_t GetPlayerID() const;
    void SetPlayerID(const uint8_t& playerID);

    std::chrono::milliseconds GetFireRate() const;
    void SetFireRate(const std::chrono::milliseconds m_fireRate);


private:
    uint8_t m_playerID;
    std::string m_username;
	size_t m_health;
    uint16_t m_score = 0;
    std::chrono::milliseconds m_fireRate;
    uint8_t m_fireRateUpgrades;
    double m_bulletSpeed;
    bool m_bulletSpeedUpgraded;
    //am eliminat points ca ar trebui sa fie doar in baza de date si upgrade-urile sa se faca pe baza informatiilor din baza de date

};

