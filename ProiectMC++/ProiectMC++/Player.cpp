#include "Player.h"

void Player::Move(char key) {
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

bool Player::IsAlive() const {
    return m_health > 0;
}

bool Player::IsEliminated() const {
    return m_health <= 0;
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

void Player::SetPlayerID(uint8_t playerID)
{
    m_playerID = playerID;
}

const std::string& Player::GetUsername() const
{
    return m_username;
}

void Player::SetPlace(uint8_t place)
{
    m_place = place;
}

uint8_t Player::GetPlace() const
{
    return m_place;
}

std::string Player::GetUsername()
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

void Player::SetScore(uint16_t score)
{
    m_score = score;
}

void Player::Respawn(const std::pair<size_t,size_t>& initialPosition) {
    m_position = initialPosition;
}

size_t Player::GetHealth() const {
    return m_health;
}

void Player::SetHealth(uint16_t health)
{
    m_health = health;
}
