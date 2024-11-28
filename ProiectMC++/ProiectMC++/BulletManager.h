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
	private:
		std::vector<std::unique_ptr<Bullet>>m_bullets;
};

