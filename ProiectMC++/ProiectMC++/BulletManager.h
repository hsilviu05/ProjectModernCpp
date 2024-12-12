#pragma once
#include <vector>
#include "Bullet.h"
#include <memory>
#include <unordered_map>
#include "Map.h"
#include "Player.h"
#include "TileType.h"
class BulletManager
{
	public:
		void UpdateBullets(Map& gameMap);
		void AddBullet(const Bullet& bullet);
		void CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap);
		void CheckBulletBulletCollisions();
		void CheckBulletPlayersCollisions(std::array<Player, 4>& players);
		bool CanShoot();
		void ShootBullet(const std::pair<size_t, size_t>& position,const Direction& direction,size_t shooterID,size_t speed);
		
	private:
		std::unordered_map<size_t, std::vector<Bullet>> m_bullets;
		std::chrono::steady_clock::time_point m_lastShotTime;
};

