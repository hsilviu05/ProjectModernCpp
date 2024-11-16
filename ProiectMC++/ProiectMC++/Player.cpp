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


Player::Player(int startX, int startY)
: m_initialPosition(startX, startY), m_health(), m_fireRate(1000)
{
}

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
bool Player::UpgradeWeapon()
{
    if (m_points >= UPGRADE_COST && m_fireRateUpgrades < MAX_FIRE_RATE_UPGRADES) {
        m_points -= UPGRADE_COST;
        m_fireRate -= FIRE_RATE_REDUCTION;
        m_fireRateUpgrades++;
        return true;
    }
    return false;
}
int Player::GetFireRate() const
{
    return m_fireRate;
}

Bullet Player::shoot() const
{
    Bullet::Position startPosition(m_initialPosition.first, m_initialPosition.second);
    return Bullet(startPosition, m_direction);
}

//Pana aici

void Player::AddScore(int points) {
    m_score += points;

    if (m_score >= 10 && !m_speedBoostApplied) {
        m_speedBoostApplied = true;
    }
}

int Player::GetScore() const {
    return m_score;
}

bool Player::CanDuplicateBulletSpeed() {
    return m_speedBoostApplied;
}


void Player::respawn() {
    m_position = m_initialPosition;
}

uint8_t Player::getHealth() const {
    return m_health;
}


