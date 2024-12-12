#include "BulletManager.h"

void BulletManager::UpdateBullets(Map& gameMap) {
    for (auto& [shooterID, bullets] : m_bullets) {
        for (auto it = bullets.begin(); it != bullets.end();) {
            auto previousPosition = it->getPosition();
            it->MoveBullet();
            CheckBulletWallCollisions(gameMap.GetWalls(), gameMap);

            if (!it->IsActive()) {
                gameMap.SetTile(previousPosition, TileType::EmptySpace);
                it = bullets.erase(it);
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
                    it = bullets.erase(it);
                }
            }
        }
    }
}

void BulletManager::AddBullet(const Bullet& bullet) {
    m_bullets[bullet.GetShooterID()].emplace_back(bullet);
}


void BulletManager::CheckBulletWallCollisions(const std::vector<Wall>& walls, Map& gameMap) {
    for (auto& [shooterID, bullets] : m_bullets) {
        for (auto& bullet : bullets) {
            if (bullet.IsActive()) {
                for (const auto& wall : walls) {
                    if (wall.getPosition() == bullet.getPosition()) {
                        if (!wall.getIsDestroyed()) {
                            if (wall.getIsDestructible()) {
                                gameMap.DestroyTile(wall.getPosition());
                            }
                            bullet.DeactivateBullet();
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

