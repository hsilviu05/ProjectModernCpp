#include "BulletManager.h"

void BulletManager::UpdateBullets()
{
    for (auto& bulletOpt : m_bullets)
    {
        if (!bulletOpt) {
            continue;
        }

        Bullet& bullet = *bulletOpt;
        auto previousPosition = bullet.getPosition();
        bullet.MoveBullet();

        if (!m_gameMap.InBounds(bullet.getPosition())) {
            bulletOpt.reset();
        }
        else {

            ProcessCollisions(bullet);

            if(bulletOpt)
				m_gameMap.SetTile(bullet.getPosition(), TileType::Bullet);
        }

        m_gameMap.SetTile(previousPosition, TileType::EmptySpace);
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
        m_gameMap.BombExplosion(position);
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

        currentBulletOpt.reset(); // Deactivate current bullet
        bulletOpt.reset(); // Deactivate other bullet
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
            player.respawn();
            bulletOpt.reset();

            m_gameMap.SetTile(bulletPosition, TileType::EmptySpace);

            m_players[shooterID].AddPoints();
            return;
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
        return;
        //throw std::runtime_error("Cannot shoot yet!");
    }
    else 
    {
        m_lastShotTime = std::chrono::steady_clock::now();
        Bullet newBullet(position, direction, shooterID, speed);
        m_bullets.emplace_back(std::move(newBullet));
    }
   
}

BulletManager::BulletManager(Map& map, std::array<Player, 4>& playerArray)
    : m_gameMap(map), m_players(playerArray) {}

void BulletManager::ProcessCollisions(std::optional<Bullet>& bulletOpt)
{
    CheckBulletWallCollisions(bulletOpt);

    if (!bulletOpt) return;
    CheckBulletBulletCollisions(bulletOpt);

    if (!bulletOpt) return;
    CheckBulletPlayersCollisions(bulletOpt);
}

