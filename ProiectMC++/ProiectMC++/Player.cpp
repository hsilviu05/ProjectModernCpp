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
: m_initialPosition(startX, startY), m_health(), m_fireRate(COOL_DOWNTIME)
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

bool Player::UpgradeBulletSpeed() {
    if (!m_bulletSpeedUpgraded && m_points >= BULLET_SPEED_MULTIPLIER_POINTS) {
        m_points -= BULLET_SPEED_MULTIPLIER_POINTS;
        m_bulletSpeed *= 2;
        m_bulletSpeedUpgraded = true;
        return true;
    }
    return false;
}

int Player::GetFireRate() const
{
    return m_fireRate;
}

  std::unique_ptr<Bullet> Player::shoot()
{
    using namespace std::chrono;

    auto now = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    if (now - m_lastShotTime >= m_fireRate) {
        m_lastShotTime = now;
        auto bullet = std::make_unique<Bullet>(m_position,m_direction,m_playerID, Bullet::DEFAULT_BULLET_SPEED);
        return bullet;
       
    }

    return nullptr;
}

size_t Player::GetPlayerID() const
{
    return m_playerID;
}

void Player::SetPlayerID(const size_t& playerID)
{
    m_playerID = playerID;
}


/* Bullet::Position startPosition(m_initialPosition.first, m_initialPosition.second);
    return Bullet(startPosition, m_direction);*/

}

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

std::vector<Bullet>& Player::GetBullets()
{
    return bullets;
}

