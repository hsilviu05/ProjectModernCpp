#pragma once

#include <vector>
#include "Wall.h"
#include "Map.h"
#include "Direction.h"
#include "GameObject.h"

class Player;

class Bullet : public GameObject
{
public:
    Bullet( size_t& shooterID, std::pair<size_t, size_t>&m_initialPosition,  Direction& direction, int b_speed = DEFAULT_BULLET_SPEED);
    Bullet()
        : GameObject(), active(false) {} //constructor pt bullet inactiv

    void MoveBullet();
    bool IsActive() const;
    void DeactivateBullet();

    static const int DEFAULT_BULLET_SPEED = 1;

    void CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap);
    void CheckBulletBulletCollisions(std::vector<Bullet>& bullets);
    void CheckBulletPlayersCollisions(std::array<Player, 4>& players);

    Direction GetDirection();
    void SetDirection(const Direction& direction);

    size_t GetShooterID() const;
    void SetShooterID(const size_t& shooterID);

private:
    size_t m_shooterID;
    std::pair<size_t, size_t>m_initialPosition;
    Direction m_direction;
    bool active;
};
