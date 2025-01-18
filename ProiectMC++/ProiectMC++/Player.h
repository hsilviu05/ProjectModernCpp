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
    
    Player(const std::string& username, std::chrono::milliseconds& fireRate, uint16_t& fireRateUpgrades, double& bulletSpeed, bool& bulletSpeedUpgraded);
    Player();
    void Move(char key);
    void TakeDamage();
    bool IsAlive() const;
    void Respawn(const std::pair<size_t, size_t>& initialPosition);
    size_t GetHealth() const;
    void SetHealth(uint16_t health);
    bool IsEliminated() const;

    double GetBulletSpeed() const;

    void AddWinBonus();
    void UpgradeFireRate();
    void AddScore();
    uint16_t GetScore() const;
    void SetScore(uint16_t score);
    void UpgradeBulletSpeed();
    uint8_t GetPlayerID() const;
    void SetPlayerID(uint8_t playerID);
    const std::string& GetUsername() const;
    void SetPlace(uint8_t place);
    uint8_t GetPlace() const;


    std::string GetUsername();

    std::chrono::milliseconds GetFireRate() const;
    void SetFireRate(const std::chrono::milliseconds m_fireRate);


private:
    uint8_t m_place;
    uint8_t m_playerID;
    std::string m_username;
	uint16_t m_health;
    uint16_t m_score;
    std::chrono::milliseconds m_fireRate;
    uint16_t m_fireRateUpgrades;
    double m_bulletSpeed;
    bool m_bulletSpeedUpgraded;
};

