#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include"Level.h"

#include <vector>

class Player {
public:
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    Player(int startX, int startY, const Level& level);

    void move(Direction dir);
    void takeDamage();
    bool isAlive() const;
    void respawn();
    int getHealth() const;
    int getX() const;
    int getY() const;
    bool isEliminated() const;

private:
    uint8_t m_x, m_y;
    uint8_t health;
    uint8_t initialX, initialY;
    Direction direction;
};

#endif // PLAYER_H


