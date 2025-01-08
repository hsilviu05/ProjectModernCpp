#include "Bullet.h"


Bullet::Bullet(const std::pair<size_t,size_t>&position,const Direction& direction, const uint8_t shooterID, const size_t speed = GameSettings::DEFAULT_BULLET_SPEED)
    : GameObject(position,speed,direction),m_shooterID(shooterID){}


void Bullet::MoveBullet()
{
    switch (m_direction) {
    case Direction::Up:m_position.first--;break;
    case Direction::Down:m_position.first++; break;
    case Direction::Left:m_position.second--; break;
    case Direction::Right:m_position.second++; break;
    }
}


uint8_t Bullet::GetShooterID() const
{
    return m_shooterID;
}

void Bullet::SetShooterID(uint8_t shooterID)
{
    m_shooterID = shooterID;
}
