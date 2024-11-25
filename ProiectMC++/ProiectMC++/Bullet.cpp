#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(const std::pair<size_t, size_t> start_position, const Direction& direction, int speed)
    : GameObject(), b_direction(direction), active(true)
{
    setPosition({ static_cast<int16_t>(start_position.first), static_cast<int16_t>(start_position.second) });
    setSpeed(speed);
}

void Bullet::MoveBullet()
{
    if (!active) return;

    std::pair<size_t, size_t> currentPosition = getPosition();
    switch (b_direction) {
    case Direction::Up:
        setPosition({ static_cast<int16_t>(currentPosition.first - m_speed), static_cast<int16_t>(currentPosition.second) });
        break;
    case Direction::Down:
        setPosition({ static_cast<int16_t>(currentPosition.first + m_speed), static_cast<int16_t>(currentPosition.second) });
        break;
    case Direction::Left:
        setPosition({ static_cast<int16_t>(currentPosition.first), static_cast<int16_t>(currentPosition.second - m_speed) });
        break;
    case Direction::Right:
        setPosition({ static_cast<int16_t>(currentPosition.first), static_cast<int16_t>(currentPosition.second + m_speed) });
        break;
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