#include "Player.h"
#include "Level.h"

/*
Player::Player(int startX, int startY, const Level& level)
    : m_x(startX), m_y(startY), initialX(startX), initialY(startY), direction(Direction::Up) {
    health = level.getPlayerHealth();
}
*/

/*
void Player::move(Direction dir) {
    direction = dir;
    switch (dir) {
    case Direction::Up:    m_y -= 1; break;
    case Direction::Down:  m_y += 1; break;
    case Direction::Left:  m_x -= 1; break;
    case Direction::Right: m_x += 1; break;
    }
}
*/


void Player::TakeDamage() {
    if (m_health > 0) {
        m_health--;
    }
}

bool Player::isAlive() const {
    return m_health > 0;
}

bool Player::isEliminated() const {
    return m_health <= 0;
}

Direction Player::getDirection() const
{
    return m_direction;
}


//adaugat de Silviu
void Player::AddPoints()
{
    m_points += POINTS_PER_ENEMY;
}

int Player::GetPoints() const
{
    return m_points;
}

void Player::AddWinBonus()
{
    m_points += WIN_BONUS;
}
//Pana aici

void Player::respawn() {
    m_position = m_initialPosition;
}

uint8_t Player::getHealth() const {
    return m_health;
}


