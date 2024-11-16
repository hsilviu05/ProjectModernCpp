#pragma once
#include "Level.h"
#include "Direction.h"
#include "Bullet.h"
#include "GameObject.h"

#include <vector>

class Player:public GameObject {
public:
    
    Player(int startX, int startY);

    void move(Direction dir);
    void TakeDamage();
    bool isAlive() const;
    void respawn();
    uint8_t getHealth() const;
    bool isEliminated() const;
    Direction getDirection() const;
    
    void AddPoints();
    int GetPoints() const;
    void AddWinBonus();
    bool UpgradeWeapon();
    int GetFireRate() const;
    void AddScore(int points);
    int GetScore() const;
    bool CanDuplicateBulletSpeed();
    Bullet shoot() const;

private:
	uint8_t m_health;
    std::pair<size_t, size_t>m_initialPosition;
    int m_points = 0;
    int m_score = 0;
    int m_fireRate;
    int m_fireRateUpgrades = 0;
    bool m_speedBoostApplied = false;

    static const int POINTS_PER_ENEMY = 100;
    static const int WIN_BONUS = 200;
    static constexpr int MAX_FIRE_RATE_UPGRADES = 4;
    static constexpr int FIRE_RATE_REDUCTION = 100;
    static constexpr int UPGRADE_COST = 500;
};

