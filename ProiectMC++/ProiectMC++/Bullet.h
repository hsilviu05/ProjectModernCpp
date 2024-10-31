#pragma once

#include "BulletPosition.h"

class Bullet
{
public:
	Bullet(const BulletPosition& b_start_position, const BulletPosition& direction,int b_speed = DEFAULT_BULLET_SPEED);

	void moveBullet();
	const BulletPosition& getBulletPosition() const;
	bool isActive() const;
	void deactivateBullet();

	static const int DEFAULT_BULLET_SPEED = 5 ;

private:

	BulletPosition b_position;	
	BulletPosition b_direction;
	int b_speed;
	bool active;
};

