﻿#include "Player.h"

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

bool Player::CanShoot() const
{
    return (std::chrono::steady_clock::now() - lastShotTime) >= cooldownTime;
}

Bullet Player::shoot()
{
    if (!CanShoot()) {
        throw std::runtime_error("Cannot shoot yet!");
    }
    lastShotTime = std::chrono::steady_clock::now();
    return Bullet(getPosition(), m_direction, m_bulletSpeed);
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

