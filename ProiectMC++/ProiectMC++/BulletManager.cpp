#include "BulletManager.h"

void BulletManager::UpdateBullets(Map& gameMap, std::array<Player, 4>& players) {
		for (auto it = m_bullets.begin(); it != m_bullets.end();) {
            auto previousPosition = it->getPosition();
            it->MoveBullet();
            CheckBulletWallCollisions(gameMap.GetWalls(), gameMap);
            CheckBulletPlayersCollisions(players);

            if (!it->IsActive()) {
                gameMap.SetTile(previousPosition, TileType::EmptySpace);
                it = m_bullets.erase(it);
            }
            else {
                if (gameMap.inBounds(it->getPosition())) {
                    gameMap.SetTile(previousPosition, TileType::EmptySpace);
                    gameMap.SetTile(it->getPosition(), TileType::Bullet);
                    ++it;
                }
                else {
                    it->DeactivateBullet();
                    gameMap.SetTile(previousPosition, TileType::EmptySpace);
                    it = m_bullets.erase(it);
                }
            }
        }
}

void BulletManager::AddBullet(const Bullet& bullet) {
    m_bullets.emplace_back(bullet);
}


void BulletManager::CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap) {

    std::unordered_map<std::pair<size_t,size_t>, const Wall*,PairHash> wallMap;
    for (const auto& wall : walls) {
        wallMap[wall.getPosition()] = &wall;
    }

    for (auto itBullet = m_bullets.begin(); itBullet != m_bullets.end();) {
		if (!itBullet->IsActive()) {
            itBullet = m_bullets.erase(itBullet);
			continue;
		}

		auto itWall = wallMap.find(itBullet->getPosition());
		if (itWall != wallMap.end()) {
			const Wall* wall = itWall->second;
			if (!wall->getIsDestroyed()) {
				if (wall->getIsDestructible()) {
					gameMap.DestroyTile(wall->getPosition());
				}
				itBullet->DeactivateBullet();
                itBullet = m_bullets.erase(itBullet);
                continue;
			}
		}
        ++itBullet;
	}
    
}

void BulletManager::CheckBulletBulletCollisions() {







    for (auto it1 = m_bullets.begin(); it1 != m_bullets.end(); ++it1) {
        for (auto& bullet1 : it1->second) {
            if (bullet1.IsActive()) {
                for (auto it2 = m_bullets.begin(); it2 != m_bullets.end(); ++it2) {
                    for (auto& bullet2 : it2->second) {
                        if (&bullet1 != &bullet2 && bullet2.IsActive() && bullet1.getPosition() == bullet2.getPosition()) {
                            bullet1.DeactivateBullet();
                            bullet2.DeactivateBullet();
                        }
                    }
                }
            }
        }
    }
}


void BulletManager::CheckBulletPlayersCollisions(std::array<Player, 4>& players) {
	for (auto& bullet : m_bullets) {
		if (bullet.IsActive()) {
			for (auto& player : players) {
				if (bullet.getPosition() == player.getPosition()) {
					if (bullet.GetShooterID() != player.GetPlayerID()) {
                            player.TakeDamage();
                            player.respawn();
                            bullet.DeactivateBullet();
                            auto itShooter = std::find_if(players.begin(), players.end(),
                                [&](const Player& p) { return p.GetPlayerID() == player.GetShooterID(); });
                            if (itShooter != players.end()) {
                                itShooter->AddPoints();
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool BulletManager::CanShoot()
{
     return  (std::chrono::steady_clock::now() - m_lastShotTime) >= GameSettings::COOL_DOWNTIME;
}

void BulletManager::ShootBullet(const std::pair<size_t, size_t>& position,const Direction& direction,size_t shooterID,size_t speed)
{
    if(!CanShoot())
    { 
        throw std::runtime_error("Cannot shoot yet!");
    }
    else 
    {
        m_lastShotTime = std::chrono::steady_clock::now();
        Bullet newBullet(position, direction, shooterID, speed);
        m_bullets[shooterID].emplace_back(std::move(newBullet));
    }
   
}

