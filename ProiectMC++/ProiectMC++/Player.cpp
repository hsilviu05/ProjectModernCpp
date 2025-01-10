#include "Player.h"

void Player::move(const char& key) {
    switch (key) {
    case 'W':  m_position.first--; break;
    case 'S':  m_position.first++; break;
    case 'A':  m_position.second--; break;
    case 'D':  m_position.second++; break;
    }
}

Player::Player(const std::string& username, std::chrono::milliseconds& fireRate, uint8_t& fireRateUpgrades, double& bulletSpeed, bool& bulletSpeedUpgraded)
    : m_score(0), m_health(3), m_username(username), m_fireRate(fireRate), m_fireRateUpgrades(fireRateUpgrades), m_bulletSpeed(bulletSpeed), m_bulletSpeedUpgraded(bulletSpeedUpgraded) {};

Player::Player()
{
}

//const std::string& GetUsername() const { return m_username; }

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
    }*/
    return false;
	
}

bool Player::UpgradeBulletSpeed() {
    /*
    if (!m_bulletSpeedUpgraded && m_points >= GameSettings::BULLET_SPEED_MULTIPLIER_POINTS) {
        m_points -= GameSettings::BULLET_SPEED_MULTIPLIER_POINTS;
        m_bulletSpeed *= 2;
        m_bulletSpeedUpgraded = true;
        return true;
    }*/
    return false;
	
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

const std::string& Player::GetUsername() const
{
    return m_username;
}

std::chrono::milliseconds Player::GetFireRate() const
{
    return m_fireRate;
}

void Player::SetFireRate(const std::chrono::milliseconds fireRate)
{
    m_fireRate = fireRate;
}


void Player::AddScore() {
    m_score += GameSettings::SCORE_PER_ENEMY;
}

uint16_t Player::GetScore() const {
    return m_score;
}

void Player::respawn(const std::pair<size_t,size_t>& initialPosition) {
    m_position = initialPosition;
}

size_t Player::getHealth() const {
    return m_health;
}
