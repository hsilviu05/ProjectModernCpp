#include "BulletManager.h"

void BulletManager::updateBullets(Map& gameMap)
{
	for (auto& bullet : m_bullets) {
		bullet->MoveBullet();
	}
}

void BulletManager::addBullet(std::unique_ptr<Bullet> bullet)
{
	m_bullets.emplace_back(std::move(bullet));
}
