#pragma once
#include <vector>
#include "Bullet.h"
#include <memory>
#include <unordered_map>
#include "Map.h"
#include "Player.h"
#include "TileType.h"
#include <vector>
class BulletManager
{
	public:
		void UpdateBullets();
		void AddBullet(const Bullet& bullet);
		void CheckBulletWallCollisions(Bullet& bullet);
		void CheckBulletBulletCollisions(Bullet& currentBullet);
		void CheckBulletPlayersCollisions(Bullet& bullet);
		bool CanShoot();
		void ShootBullet(const std::pair<size_t, size_t>& position,const Direction& direction,size_t shooterID,size_t speed);
		BulletManager(Map& map, std::array<Player, 4>& playerArray);
		
	private:
		Map& m_gameMap;
		std::array<Player, 4>& m_players;
		void ProcessCollisions(Bullet& bullet);
		std::vector<Bullet> m_bullets;
		std::chrono::steady_clock::time_point m_lastShotTime;
		struct PairHash {
			std::size_t operator()(const std::pair<int, int>& p) const {
				return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
			}
		};
};

