#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(const Position& start_position,const Direction& direction, int speed )
: b_position(start_position), b_direction(direction), b_speed(speed), active(true){}


void Bullet::MoveBullet()
{
	if (!active) return;

	switch (b_direction) {
	case Direction::Up:
		b_position.SetPosition(b_position.getPosition().first - b_speed, b_position.getPosition().second);
		break;
	case Direction::Down:
		b_position.SetPosition(b_position.getPosition().first + b_speed, b_position.getPosition().second);
		break;
	case Direction::Left:
		b_position.SetPosition(b_position.getPosition().first, b_position.getPosition().second - b_speed);
		break;
	case Direction::Right:
		b_position.SetPosition(b_position.getPosition().first, b_position.getPosition().second + b_speed);
		break;
	}
	//CheckBulletWallCollisions(walls, gameMap);
	//CheckBulletBulletCollisions(bullets);
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


void Bullet::CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap)
{
		if (!active)
		{
		return;
		}

		bool collisionDetected = false;
		for (const auto& wall : walls)
		{
			if (wall.getPosition() == b_position.getPosition()) {
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
			if (b_position.getPosition() == otherBullet.GetBulletPosition().getPosition())
			{
				DeactivateBullet();
				otherBullet.DeactivateBullet();
				break;
			}
		}
	}
}

void Bullet::CheckBulletPlayersCollisions(std::array<Player,4>& players)
{
	for (auto& player : players)
	{
		if (b_position.getPosition() == player.getPosition())
		{
			player.TakeDamage();
			player.respawn();
			DeactivateBullet();
			player.AddPoints();
		}
	}
}