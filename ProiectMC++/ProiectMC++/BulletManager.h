#pragma once
#include <vector>
#include "Bullet.h"
#include <memory>
#include "Map.h"
#include "Player.h"
class BulletManager
{
	public:
		void updateBullets(Map& gameMap);
		void addBullet(std::unique_ptr<Bullet>bullet);
	private:
		std::vector<std::unique_ptr<Bullet>>m_bullets;
};

