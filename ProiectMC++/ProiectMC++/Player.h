#pragma once
import GameObject;
import GameSettings;
import Direction;
#include "Bullet.h"
#include <chrono>
#include <memory>
#include <vector>

class Player:public GameObject {
public:
    
    Player(const std::string& username, std::chrono::milliseconds& fireRate, uint8_t& fireRateUpgrades, double& bulletSpeed, bool& bulletSpeedUpgraded);
    Player();
    void Move(const char& key);
    void TakeDamage();
    bool IsAlive() const;
    void Respawn(const std::pair<size_t, size_t>& initialPosition);
    size_t GetHealth() const;
    bool IsEliminated() const;

    void AddWinBonus();
    bool UpgradeWeapon();
    void AddScore();
    uint16_t GetScore() const;
    bool UpgradeBulletSpeed();
    uint8_t GetPlayerID() const;
    void SetPlayerID(const uint8_t& playerID);
    const std::string& GetUsername() const;

    std::chrono::milliseconds GetFireRate() const;
    void SetFireRate(const std::chrono::milliseconds m_fireRate);


private:
    uint8_t m_playerID;
    std::string m_username;
	uint16_t m_health;
    uint16_t m_score = 0;
    std::chrono::milliseconds m_fireRate;
    uint8_t m_fireRateUpgrades;
    double m_bulletSpeed;
    bool m_bulletSpeedUpgraded;
    

};

