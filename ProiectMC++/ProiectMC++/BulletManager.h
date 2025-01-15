#pragma once
#include "Bullet.h"
#include "Map.h"
#include "Player.h"
#include "TileType.h"
#include "EntityFactory.h"

#include <vector>
#include <unordered_map>

#include <optional>
#include <algorithm>
#include <ranges>
#include <memory>


class BulletManager
{
	public:
		
		void UpdateBullets();
		void CheckBulletWallCollisions(std::optional<Bullet>& bulletOpt);
		void CheckBulletBulletCollisions(std::optional<Bullet>& currentBulletOpt);
		void CheckBulletPlayersCollisions(std::optional<Bullet>& bulletOp);
		bool CanShoot(const size_t& shooterID);
		void ShootBullet(const std::pair<size_t, size_t>& position,const Direction& direction, const uint8_t shooterID,size_t speed);
		BulletManager(Map& map, std::array < std::shared_ptr <Player>, 4>& m_players);
		
	private:
		Map& m_gameMap;
		std::array < std::shared_ptr <Player> ,4> &m_players;
		void ProcessCollisions(std::optional<Bullet>& bulletOpt);
		void BombExplosion(const std::pair<size_t, size_t>& bombPosition);
		std::vector<std::optional<Bullet>> m_bullets;
		std::unordered_map<size_t, std::chrono::steady_clock::time_point> m_lastShotTime;
		bool HandlePortal(Bullet& bullet, const std::pair<size_t, size_t>& previousPosition);
};

