#include "Player.h"
#include "Level.h"

Player::Player(int startX, int startY, const Level& level)
    : m_x(startX), m_y(startY), initialX(startX), initialY(startY), direction(Direction::Up) {
    health = level.getPlayerHealth();
}
    
void Player::move(Direction dir) {
    direction = dir;
    switch (dir) {
    case Direction::Up:    m_y -= 1; break;
    case Direction::Down:  m_y += 1; break;
    case Direction::Left:  m_x -= 1; break;
    case Direction::Right: m_x += 1; break;
    }
}


void Player::takeDamage() {
    if (health > 0) {
        health--;
    }
}

bool Player::isAlive() const {
    return health > 0;
}

bool Player::isEliminated() const {
    return health <= 0;
}

std::pair<int, int> Player::getPosition() const
{
    return std::pair<int, int>();
}

Direction Player::getDirection() const
{
    return Direction();
}


void Player::respawn() {
    m_x = initialX;
    m_y = initialY;
}

int Player::getX() const { return m_x; }

int Player::getY() const { return m_y; }

int Player::getHealth() const {
    return health;
}

