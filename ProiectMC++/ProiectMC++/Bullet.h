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
    Bullet(const std::pair<size_t, size_t>m_initialPosition, const Direction& direction, int b_speed = DEFAULT_BULLET_SPEED);
    Bullet()
        : GameObject(), active(false) {} //constructor pt bullet inactiv

    void MoveBullet();
    bool IsActive() const;
    void DeactivateBullet();

    static const int DEFAULT_BULLET_SPEED = 1;

    void CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap);
    void CheckBulletBulletCollisions(std::vector<Bullet>& bullets);
    void CheckBulletPlayersCollisions(std::array<Player, 4>& players);

private:
    std::pair<size_t, size_t>m_initialPosition;
    Direction b_direction;
    bool active;
};
