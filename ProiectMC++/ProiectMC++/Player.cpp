#include "Player.h"

void Player::move(const char& key) {
    switch (key) {
    case 'W':  m_position.first--; break;
    case 'S':  m_position.first++; break;
    case 'A':  m_position.second--; break;
    case 'D':  m_position.second++; break;
    }
}

Player::Player(int startX, int startY)
: m_initialPosition(startX, startY), m_health(), m_fireRate(GameSettings::COOL_DOWNTIME)
{
}

Player::Player()
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

void Player::setDirection(Direction playerDirection)
{
    m_direction = playerDirection;
}


void Player::AddWinBonus()
{
    m_score += GameSettings::WIN_BONUS;
}
bool Player::UpgradeWeapon()
{
    /*
    if (m_points >= GameSettings::UPGRADE_COST && m_fireRateUpgrades < GameSettings::MAX_FIRE_RATE_UPGRADES) {
        m_points -= GameSettings::UPGRADE_COST;
        m_fireRate -= GameSettings::FIRE_RATE_REDUCTION;
        m_fireRateUpgrades++;
        return true;
    }
    return false;
	*/
}

bool Player::UpgradeBulletSpeed() {
    /*
    if (!m_bulletSpeedUpgraded && m_points >= GameSettings::BULLET_SPEED_MULTIPLIER_POINTS) {
        m_points -= GameSettings::BULLET_SPEED_MULTIPLIER_POINTS;
        m_bulletSpeed *= 2;
        m_bulletSpeedUpgraded = true;
        return true;
    }
    return false;
	*/
    //direct cu baza de date
}

uint8_t Player::GetPlayerID() const
{
    return m_playerID;
}

void Player::SetPlayerID(const uint8_t& playerID)
{
    m_playerID = playerID;
}


void Player::AddScore() {
    m_score += GameSettings::SCORE_PER_ENEMY;
}

uint16_t Player::GetScore() const {
    return m_score;
}

bool Player::CanDuplicateBulletSpeed() {
    return m_speedBoostApplied;
}


void Player::respawn(const std::pair<size_t,size_t>& initialPosition) {
    m_position = initialPosition;
}

size_t Player::getHealth() const {
    return m_health;
}
