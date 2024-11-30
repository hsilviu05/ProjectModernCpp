#pragma once
#include "Level.h"
#include "Direction.h"
#include "Bullet.h"
#include "GameObject.h"
#include "Constants.h"
#include <chrono>
#include <memory>


#include <vector>

class Player:public GameObject {
public:
    
    Player(int startX, int startY);
    Player();
    void move(const char& key);
    void TakeDamage();
    bool isAlive() const;
    void respawn();
    uint8_t getHealth() const;
    bool isEliminated() const;
    Direction getDirection() const;
    void setDirection(Direction player_direction);

    void AddPoints();
    int GetPoints() const;
    void AddWinBonus();
    bool UpgradeWeapon();
    int GetFireRate() const;
    void AddScore(int points);
    int GetScore() const;
    bool CanDuplicateBulletSpeed();
    bool UpgradeBulletSpeed();
    std::unique_ptr<Bullet> shoot();
    size_t GetPlayerID() const;
    void SetPlayerID(const size_t& playerID);

private:
    size_t m_playerID;
    std::vector<std::unique_ptr<Bullet>>bullets;
	uint8_t m_health;
    std::pair<size_t, size_t>m_initialPosition;
    int m_points = 0;
    int m_score = 0;
    int m_fireRate;
    int m_fireRateUpgrades = 0;
    double m_bulletSpeed = 0.25;
    std::chrono::milliseconds cooldownTime = std::chrono::milliseconds(COOL_DOWNTIME);
    std::chrono::steady_clock::time_point lastShotTime;
    bool m_speedBoostApplied = false;
    bool m_bulletSpeedUpgraded = false;

};

