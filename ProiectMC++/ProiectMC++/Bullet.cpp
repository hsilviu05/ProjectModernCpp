#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(const Position& start_position, Direction& direction, int speed)
: b_position(start_position), b_direction(direction), b_speed(speed), active(true){}


void Bullet::MoveBullet()
{
	if (!active) return;

	switch (b_direction) {
	case Direction::Up:
		b_position.SetPosition(b_position.getPosition().first, b_position.getPosition().second - b_speed);
		break;
	case Direction::Down:
		b_position.SetPosition(b_position.getPosition().first, b_position.getPosition().second + b_speed);
		break;
	case Direction::Left:
		b_position.SetPosition(b_position.getPosition().first - b_speed, b_position.getPosition().second);
		break;
	case Direction::Right:
		b_position.SetPosition(b_position.getPosition().first + b_speed, b_position.getPosition().second);
		break;
	}
}


const Bullet::Position& Bullet::GetBulletPosition() const
{
	return b_position;
}

bool Bullet::IsActive() const
{
	return active;
}

void Bullet::DeactivateBullet()
{
	active = false;
}


bool Bullet::CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap)
{
		if (!active) {
		return false;
	}

	for (const auto& wall : walls)
	{
		if (wall.getPosition() == b_position.getPosition() && !wall.getIsDestroyed())
		{
			if (wall.getIsDestructible())
			{
				gameMap.destroyTile(wall.getPosition().first, wall.getPosition().second);
			}
			DeactivateBullet();
			return true;
		}
	}
	return false;
}

void Bullet::CheckBulletPlayersCollisions(std::vector<Player>& players)
{
	for (auto& player : players)
	{
		if (b_position.getPosition() == player.getPosition()) 
		{
			player.takeDamage();
			DeactivateBullet();
		}
	}
}