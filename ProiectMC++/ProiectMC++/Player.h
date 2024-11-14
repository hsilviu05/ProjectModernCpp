#pragma once
#include "Level.h"
#include "Direction.h"
#include "Bullet.h"
#include "GameObject.h"

#include <vector>

class Player:public GameObject {
public:
    
    Player(int startX, int startY, const Level& level);

    void move(Direction dir);
    void takeDamage();
    bool isAlive() const;
    void respawn();
    uint8_t getHealth() const;
    bool isEliminated() const;
    Direction getDirection() const;


private:
	uint8_t m_health;
    std::pair<size_t, size_t>m_initialPosition;
};

