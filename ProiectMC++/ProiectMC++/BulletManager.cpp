#include "BulletManager.h"

void BulletManager::UpdateBullets(Map& gameMap)
{
	for (auto& bullet : m_bullets) {
		std::pair<size_t, size_t>previousPosition=bullet->getPosition();
		bullet->MoveBullet();
		CheckBulletWallCollisions(gameMap.GetWalls(), gameMap);
	if (!bullet->IsActive()){
		gameMap.SetTile(previousPosition, TileType::EmptySpace);
	}
	else {
		if (gameMap.inBounds(bullet->getPosition())) {
			gameMap.SetTile(previousPosition, TileType::EmptySpace);
			gameMap.SetTile(bullet->getPosition(), TileType::Bullet);
		}
		else {

			bullet->DeactivateBullet();
			gameMap.SetTile(previousPosition, TileType::EmptySpace);
		}
	}
}

m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
	[](const std::unique_ptr<Bullet>& bullet) {
		return !bullet->IsActive();
	}),m_bullets.end());
}

void BulletManager::AddBullet(std::unique_ptr<Bullet> bullet)
{
	m_bullets.emplace_back(std::move(bullet));
}

void BulletManager::CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap)
{
	for (auto& bullet : m_bullets) {
		if (bullet->IsActive()) {
			bool collisionDetected = false;
			for (const auto& wall : walls) {
				if (wall.getPosition() == bullet->getPosition()) {
					if (!wall.getIsDestroyed()) {
						collisionDetected = true;
						if (wall.getIsDestructible()) {
							gameMap.DestroyTile(wall.getPosition());
						}
						else {
							bullet->DeactivateBullet();
						}
						break;
					}
				}
			}
			if (collisionDetected) {
				bullet->DeactivateBullet();
			}
		}
	}
}

void BulletManager::CheckBulletBulletCollisions(std::vector<Bullet>& bullets)
{
	for (auto& bullet : m_bullets) {
		if (bullet->IsActive()) {
			for (auto& otherBullet : m_bullets) {
				if (&otherBullet != &bullet && otherBullet->IsActive() && bullet->getPosition() == otherBullet->getPosition()) {
					bullet->DeactivateBullet();
					otherBullet->DeactivateBullet();
					break;
				}
			}
		}
	}
}

void BulletManager::CheckBulletPlayersCollisions(std::array<Player, 4>& players)
{
	for (auto& bullet : m_bullets) {
		if (bullet->IsActive()) {
			for (auto& player : players) {
				if (bullet->getPosition() == player.getPosition()) {
					player.TakeDamage();
					player.respawn();
					bullet->DeactivateBullet();
					player.AddPoints();
					break;
				}
			}
		}
	}
}

