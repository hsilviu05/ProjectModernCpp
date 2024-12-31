#pragma once
#include "Level.h"
#include "Direction.h"
#include "Bullet.h"
#include "GameObject.h"
#include "GameSettings.h"
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
    void respawn(const std::pair<size_t, size_t>& initialPosition);
    size_t getHealth() const;
    bool isEliminated() const;
    Direction getDirection() const;
    void setDirection(Direction player_direction);

    void AddWinBonus();
    bool UpgradeWeapon();
    void AddScore();
    uint16_t GetScore() const;
    bool CanDuplicateBulletSpeed();
    bool UpgradeBulletSpeed();
    uint8_t GetPlayerID() const;
    void SetPlayerID(const uint8_t& playerID);

private:
    uint8_t m_playerID;
    std::string m_username;
	size_t m_health;
    uint16_t m_score = 0;
    std::chrono::milliseconds m_fireRate;
    int m_fireRateUpgrades = 0;
    double m_bulletSpeed = 0.25;
    bool m_speedBoostApplied = false;
    bool m_bulletSpeedUpgraded = false;
    //am eliminat points ca ar trebui sa fie doar in baza de date si upgrade-urile sa se faca pe baza informatiilor din baza de date

};

