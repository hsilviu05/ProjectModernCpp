#pragma once

#include "BulletPosition.h"
#include <vector>
#include "Wall.h"
#include "Player.h"
#include "Map.h"
#include "Direction.h"

class Bullet
{
public:
	Bullet(const BulletPosition& b_start_position, Direction& direction,int b_speed = DEFAULT_BULLET_SPEED);

	void moveBullet();
	const BulletPosition& getBulletPosition() const;
	bool isActive() const;
	void deactivateBullet();

	static const int DEFAULT_BULLET_SPEED = 5 ;

	bool checkBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap);
	void checkBulletPlayersCollisions(std::vector<Player>& players);
private:

	BulletPosition b_position;	
	Direction b_direction;
	int b_speed;
	bool active;
};

