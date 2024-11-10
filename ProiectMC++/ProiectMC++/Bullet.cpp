#include "Bullet.h"

Bullet::Bullet(const BulletPosition& start_position, Direction& direction, int speed)
: b_position(start_position), b_direction(direction), b_speed(speed), active(true){}

/*
void Bullet::moveBullet()
{
	if (active) {
		std::pair<int, int> current_pos = b_position.getPosition();
		b_position.SetPosition(current_pos.first + b_direction.getPosition().first * b_speed,
			current_pos.second + b_direction.getPosition().second * b_speed);

	}
}
*/

const BulletPosition& Bullet::getBulletPosition() const
{
	return b_position;
}

bool Bullet::isActive() const
{
	return active;
}

void Bullet::deactivateBullet()
{
	active = false;
}


bool Bullet::checkBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap)
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
			deactivateBullet();
			return true;
		}
	}
	return false;
}

void Bullet::checkBulletPlayersCollisions(std::vector<Player>& players)
{
	for (auto& player : players)
	{
		if (b_position.getPosition() == player.getPosition()) 
		{
			player.takeDamage();
			deactivateBullet();
		}
	}
}