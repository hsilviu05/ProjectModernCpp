#pragma once
/*
#include <vector>
#include "Wall.h"
#include "Map.h"
#include "Direction.h"

class Player;

class Bullet
{
public:
	struct Position 
	{
		int x, y;

		Position(int x_cord, int y_cord) : x(x_cord), y(y_cord) {}

		std::pair<int, int> getPosition() const { return { x, y }; }
		void SetPosition(int x_cord, int y_cord) { x = x_cord; y = y_cord; }

		bool operator==(const Position& other) const {
			return x == other.x && y == other.y;
		}
	};

public:
	Bullet(const Position& b_start_position, Direction& direction,int b_speed = DEFAULT_BULLET_SPEED);

	void MoveBullet();
	const Position& GetBulletPosition() const;
	bool IsActive() const;
	void DeactivateBullet();

	static const int DEFAULT_BULLET_SPEED = 5 ;

	bool CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap);
	void CheckBulletPlayersCollisions(std::vector<Player>& players);
private:

	Position b_position;	
	Direction b_direction;
	int b_speed;
	bool active;
};
*/