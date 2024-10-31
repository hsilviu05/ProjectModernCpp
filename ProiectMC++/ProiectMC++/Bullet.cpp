#include "Bullet.h"

Bullet::Bullet(const BulletPosition& start_position, const BulletPosition& direction, int speed)
: b_position(start_position), b_direction(direction), b_speed(speed), active(true){}

void Bullet::moveBullet()
{
	if (active) {
		b_position.cord_x += b_direction.cord_x * b_speed;
		b_position.cord_x += b_direction.cord_x * b_speed;
	}
}

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
