#pragma once
#include <vector>
#include "Bullet.h"
#include <memory>
#include "Map.h"
#include "Player.h"
#include "TileType.h"
class BulletManager
{
	public:
		void UpdateBullets(Map& gameMap);
		void AddBullet(std::unique_ptr<Bullet>bullet);
		void CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap);
		void CheckBulletBulletCollisions(std::vector<Bullet>& bullets);
		void CheckBulletPlayersCollisions(std::array<Player, 4>& players);
	private:
		std::vector<std::unique_ptr<Bullet>>m_bullets;
};

