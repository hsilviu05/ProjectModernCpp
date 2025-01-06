﻿#include "BulletManager.h"


void BulletManager::UpdateBullets()
{
    for (auto& bulletOpt : m_bullets)
    {
        if (!bulletOpt) {
            continue;
        }

        Bullet& bullet = *bulletOpt;
        size_t m_bulletspeed = GameSettings::DEFAULT_BULLET_SPEED;
        auto currentPosition = bullet.getPosition();

        for (size_t step = 0; step < m_bulletspeed; ++step) {
            
            auto previousPosition = currentPosition;
            bullet.MoveBullet();
            currentPosition = bullet.getPosition();

            if (!m_gameMap.InBounds(currentPosition))
            {
                bulletOpt.reset();
                m_gameMap.SetTile(previousPosition, TileType::EmptySpace);
                break;
            }

            ProcessCollisions(bulletOpt);
            if (!bulletOpt)
            {
                m_gameMap.SetTile(previousPosition, TileType::EmptySpace);
                break;
            }

            m_gameMap.SetTile(currentPosition, TileType::Bullet);
            m_gameMap.SetTile(previousPosition, TileType::EmptySpace);
        }
    }
    std::erase_if(m_bullets, [](const std::optional<Bullet>& bulletOpt) {
    	return !bulletOpt;
    });
}

void BulletManager::AddBullet(const Bullet& bullet) {
    m_bullets.emplace_back(bullet);
}


void BulletManager::CheckBulletWallCollisions(std::optional<Bullet>& bulletOpt) {

    Bullet& bullet = *bulletOpt;
	auto position = bullet.getPosition();
	auto tileType = m_gameMap.GetTile(position);

    switch (tileType) {
    case TileType::DestrucitbleWall:
        bulletOpt.reset();
        m_gameMap.SetTile(position, TileType::EmptySpace);
        break;

    case TileType::DestrucitbleWallWithBomb:
        bulletOpt.reset();
        BombExplosion(position);
        break;

    case TileType::IndestrucitbleWall:
        bulletOpt.reset();
        break;

    default:
        break;
    }
}

void BulletManager::CheckBulletBulletCollisions(std::optional<Bullet>& currentBulletOpt) {


    Bullet& currentBullet = *currentBulletOpt;
    auto currentPosition = currentBullet.getPosition();

    for (auto& bulletOpt : m_bullets) 
    {
        if (!bulletOpt) continue;
        Bullet& bullet = *bulletOpt;

        if (&currentBullet == &bullet || bullet.getPosition() != currentPosition) {
            continue;
        }

        currentBulletOpt.reset(); 
        bulletOpt.reset(); 
        m_gameMap.SetTile(currentPosition, TileType::EmptySpace);
        return;
    }
}


void BulletManager::CheckBulletPlayersCollisions(std::optional<Bullet>& bulletOpt) {

    Bullet& bullet = *bulletOpt;
    const auto bulletPosition = bullet.getPosition();
    const auto shooterID = bullet.GetShooterID();

    for (auto& player : m_players) {

        if (bulletPosition == player.getPosition() && shooterID != player.GetPlayerID()) {
            player.TakeDamage();
            player.respawn(m_gameMap.getStartPosition(player.GetPlayerID()));
            bulletOpt.reset();

            m_gameMap.SetTile(bulletPosition, TileType::EmptySpace);

            //m_players[shooterID].AddPoints();
            return;
        }
    }
}

bool BulletManager::CanShoot(const size_t& shooterID)
{
    auto now = std::chrono::steady_clock::now();
    if (m_lastShotTime.find(shooterID) == m_lastShotTime.end()) {
        return true;
    }
    return (now - m_lastShotTime[shooterID]) >= GameSettings::COOL_DOWNTIME;
	//return  (std::chrono::steady_clock::now() - m_lastShotTime) >= GameSettings::COOL_DOWNTIME;
}

void BulletManager::ShootBullet(const std::pair<size_t, size_t>& position,const Direction& direction,const size_t& shooterID,size_t speed)
{
    if(!CanShoot(shooterID))
    {
        
    }
    else
    {
        m_lastShotTime[shooterID] = std::chrono::steady_clock::now();
        std::optional<Bullet> newBullet = Bullet(position, direction, shooterID, speed);
        m_bullets.emplace_back(newBullet);

       // m_lastShotTime = std::chrono::steady_clock::now();
	}
}

BulletManager::BulletManager(Map& map, std::array<Player, 4>& playerArray)
    : m_gameMap(map), m_players(playerArray) {}

void BulletManager::ProcessCollisions(std::optional<Bullet>& bulletOpt)
{
    CheckBulletWallCollisions(bulletOpt);

    //if (!bulletOpt) return;
    //CheckBulletBulletCollisions(bulletOpt);

    //if (!bulletOpt) return;
    //CheckBulletPlayersCollisions(bulletOpt);
}


void BulletManager::BombExplosion(const std::pair<size_t, size_t>& bombPosition)
{
    // for (int x = bombPosition.first - radius; x <= bombPosition.first + radius; ++x)
    //for (int y = bombPosition.second - radius; y <= bombPosition.second + radius; ++y)
    const int radius = 10;
    const int radiusSquared = radius * radius;

     for (auto x = std::max(0, static_cast<int>(bombPosition.first - radius));
        x <= bombPosition.first + radius && x < m_gameMap.getHeight(); ++x) {
        for (auto y = std::max(0, static_cast<int>(bombPosition.second - radius));
        y <= bombPosition.second + radius && y < m_gameMap.getWidth(); ++y)
        {
            int dx = x - bombPosition.first;
            int dy = y - bombPosition.second;

            if (dx * dx + dy * dy <= radiusSquared && m_gameMap.InBounds({ x, y })) {
                auto tileType = m_gameMap.GetTile({ x, y });

                if (tileType == TileType::DestrucitbleWall || tileType == TileType::DestrucitbleWallWithBomb) {
                    m_gameMap.SetTile({ x, y }, TileType::EmptySpace);

                    if (tileType == TileType::DestrucitbleWallWithBomb) {
                        BombExplosion({ x, y });
                    }
                }

                for (auto& player : m_players) {
                    if (player.getPosition() == std::make_pair(x, y)) {
                        player.TakeDamage();
                        player.respawn(m_gameMap.getStartPosition(player.GetPlayerID()));
                    }
                }
            }
        }
    }
}



/*
void BulletManager::BombExplosion(const std::pair<size_t, size_t>& bombPosition)
{
    const int radius = 10;
    const int radiusSquared = radius * radius;

    for (auto x = std::max(0, static_cast<int>(bombPosition.first - radius));
        x <= bombPosition.first + radius && x < m_gameMap.getHeight(); ++x) {
        for (int y = std::max(0, static_cast<int>(bombPosition.second - radius));
            y <= bombPosition.second + radius && y < m_gameMap.getWidth(); ++y) {

            int dx = x - bombPosition.first;
            int dy = y - bombPosition.second;

            if (dx * dx + dy * dy <= radiusSquared && m_gameMap.InBounds({ static_cast<size_t>(x), static_cast<size_t>(y) })) {
                auto tileType = m_gameMap.GetTile({ static_cast<size_t>(x), static_cast<size_t>(y) });

                if (tileType == TileType::DestrucitbleWall || tileType == TileType::DestrucitbleWallWithBomb) {
                    m_gameMap.SetTile({ static_cast<size_t>(x), static_cast<size_t>(y) }, TileType::EmptySpace);

                    if (tileType == TileType::DestrucitbleWallWithBomb) {
                        BombExplosion({ static_cast<size_t>(x), static_cast<size_t>(y) });
                    }
                }

                for (auto& player : m_players) {
                    if (player.getPosition() == std::make_pair(static_cast<size_t>(x), static_cast<size_t>(y))) {
                        player.TakeDamage();
                        player.respawn(m_gameMap.getStartPosition(player.GetPlayerID()));
                    }
                }
            }
        }
    }
}

*/
