#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(const std::pair<size_t,size_t>&position,const Direction& direction,const size_t& shooterID, const size_t& speed = DEFAULT_BULLET_SPEED)
    : GameObject(position,speed,direction),m_shooterID(shooterID),active(true){}


void Bullet::MoveBullet()
{
    if (!active) return;

    std::pair<size_t, size_t> currentPosition = getPosition();
    switch (m_direction) {
    case Direction::Up:m_position.first--;break;
    case Direction::Down:m_position.first++; break;
    case Direction::Left:m_position.second--; break;
    case Direction::Right:m_position.second++; break;
    }
}

bool Bullet::IsActive() const
{
    return active;
}

void Bullet::DeactivateBullet()
{
    active = false;
}

void Bullet::CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap)
{
    if (!active)
    {
        return;
    }

    bool collisionDetected = false;
    for (const auto& wall : walls)
    {
        if (wall.getPosition() == getPosition()) {
            if (!wall.getIsDestroyed()) {
                collisionDetected = true;
                if (wall.getIsDestructible()) {
                    gameMap.DestroyTile(wall.getPosition());
                }
                else {
                    DeactivateBullet();
                }
                break;
            }
        }
    }
    if (collisionDetected)
    {
        DeactivateBullet();
    }
}


void Bullet::CheckBulletBulletCollisions(std::vector<Bullet>& bullets)
{
    if (!active)
        return;
    for (auto& otherBullet : bullets)
    {
        if (&otherBullet != this && otherBullet.IsActive())
        {
            if (getPosition() == otherBullet.getPosition())
            {
                DeactivateBullet();
                otherBullet.DeactivateBullet();
                break;
            }
        }
    }
}

void Bullet::CheckBulletPlayersCollisions(std::array<Player, 4>& players)
{
    for (auto& player : players)
    {
        if (getPosition() == player.getPosition())
        {
            player.TakeDamage();
            player.respawn();
            DeactivateBullet();
            player.AddPoints();
            return;
        }
    }
}

Direction Bullet::GetDirection()
{
    return m_direction;
}

void Bullet::SetDirection(const Direction& direction)
{
    m_direction = direction;
}

size_t Bullet::GetShooterID() const
{
    return m_shooterID;
}

void Bullet::SetShooterID(const size_t& shooterID)
{
    m_shooterID = shooterID;
}
