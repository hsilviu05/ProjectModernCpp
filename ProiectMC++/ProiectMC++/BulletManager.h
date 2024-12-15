#pragma once
#include <vector>
#include "Bullet.h"
#include <memory>
#include <unordered_map>
#include "Map.h"
#include "Player.h"
#include "TileType.h"
#include <list>
class BulletManager
{
	public:
		void UpdateBullets(Map& gameMap, std::array<Player, 4>& players);
		void AddBullet(const Bullet& bullet);
		void CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap);
		void CheckBulletBulletCollisions();
		void CheckBulletWithWall(Map& gameMap);
		void CheckBulletPlayersCollisions(std::array<Player, 4>& players);
		bool CanShoot();
		void ShootBullet(const std::pair<size_t, size_t>& position,const Direction& direction,size_t shooterID,size_t speed);
		
	private:
		std::list<Bullet> m_bullets;
		std::chrono::steady_clock::time_point m_lastShotTime;
		struct PairHash {
			std::size_t operator()(const std::pair<int, int>& p) const {
				return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
			}
		};
};

