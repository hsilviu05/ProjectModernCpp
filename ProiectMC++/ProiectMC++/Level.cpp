#include "Level.h"
#include "Player.h"

Player::Player(int startX, int startY,int health, const Level& level)
    : m_x(startX), m_y(startY), initialX(startX), initialY(startY), health(level.getPlayerHealth()), direction(Direction::Up) {}

// Restul metodelor rămân la fel
void Player::move(Direction dir) {
    direction = dir;
    switch (dir) {
    case Direction::Up:    m_y -= 1; break;
    case Direction::Down:  m_y += 1; break;
    case Direction::Left:  m_x -= 1; break;
    case Direction::Right: m_x += 1; break;
    }
}

int Player::getX() const { return m_x; }

int Player::getY() const { return m_y; }
