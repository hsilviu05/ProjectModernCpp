#include "BulletManager.h"

void BulletManager::UpdateBullets(Map& gameMap)
{
	for (auto& bullet : m_bullets) {
		std::pair<size_t, size_t>previousPosition=bullet->getPosition();
		bullet->MoveBullet();
		if (gameMap.inBounds(bullet->getPosition()) == true) {
			if (gameMap.GetTile(previousPosition) != TileType::Player){
				gameMap.SetTile(previousPosition, TileType::EmptySpace);
			}
			if (gameMap.GetTile(bullet->getPosition()) == TileType::DestrucitbleWall && bullet->IsActive()==true)
			{
				gameMap.SetTile(bullet->getPosition(), TileType::EmptySpace);
				bullet->DeactivateBullet();
			}
			if (gameMap.GetTile(bullet->getPosition()) == TileType::IndestrucitbleWall && bullet->IsActive() == true)
			{
				bullet->DeactivateBullet();
			}
			if (gameMap.GetTile(bullet->getPosition()) != TileType::IndestrucitbleWall || gameMap.GetTile(bullet->getPosition()) != TileType::DestrucitbleWall)
				if(bullet->IsActive() == true)
					gameMap.SetTile(bullet->getPosition(), TileType::Bullet);
		}
		else
		{
			gameMap.SetTile(previousPosition, TileType::EmptySpace);
			bullet->DeactivateBullet();
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
