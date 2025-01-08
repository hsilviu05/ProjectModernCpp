#pragma once
#include "Bullet.h"
#include "Map.h"
#include "Player.h"
#include "TileType.h"

#include <vector>
#include <unordered_map>

#include <optional>
#include <algorithm>
#include <ranges>


class BulletManager
{
	public:
		
		void UpdateBullets();
		void AddBullet(const Bullet& bullet);
		void CheckBulletWallCollisions(std::optional<Bullet>& bulletOpt);
		void CheckBulletBulletCollisions(std::optional<Bullet>& currentBulletOpt);
		void CheckBulletPlayersCollisions(std::optional<Bullet>& bulletOp);
		bool CanShoot(const size_t& shooterID);
		void ShootBullet(const std::pair<size_t, size_t>& position,const Direction& direction,const size_t& shooterID,size_t speed);
		BulletManager(Map& map, std::array<Player, 4>& playerArray);
		
	private:
		Map& m_gameMap;
		std::array<Player, 4>& m_players;
		void ProcessCollisions(std::optional<Bullet>& bulletOpt);
		void BombExplosion(const std::pair<size_t, size_t>& bombPosition);
		std::vector<std::optional<Bullet>> m_bullets;
		std::unordered_map<size_t, std::chrono::steady_clock::time_point> m_lastShotTime;
		//std::chrono::steady_clock::time_point m_lastShotTime;
};

