#include "BulletManager.h"

void BulletManager::UpdateBullets()
{
    for (auto& bullet : m_bullets)
    {
        if (!bullet.IsActive()) {
            continue;
        }
        auto previousPosition = bullet.getPosition();
        bullet.MoveBullet();

        if (!m_gameMap.inBounds(bullet.getPosition())) {
            bullet.DeactivateBullet();
        }
        else {

            ProcessCollisions(bullet);

            if(bullet.IsActive())
				m_gameMap.SetTile(bullet.getPosition(), TileType::Bullet);
        }

        m_gameMap.SetTile(previousPosition, TileType::EmptySpace);
    }

    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(), [](const Bullet& bullet){
            return !bullet.IsActive(); }),m_bullets.end());
}

void BulletManager::AddBullet(const Bullet& bullet) {
    m_bullets.emplace_back(bullet);
}


void BulletManager::CheckBulletWallCollisions(Bullet& bullet) {

	auto position = bullet.getPosition();
	auto tileType = m_gameMap.GetTile(position);

    switch (tileType) {
    case TileType::DestrucitbleWall:
        bullet.DeactivateBullet();
        m_gameMap.SetTile(position, TileType::EmptySpace);
        break;

    case TileType::DestrucitbleWallWithBomb:
        bullet.DeactivateBullet();
        m_gameMap.BombExplosion(position);
        break;

    case TileType::IndestrucitbleWall:
        bullet.DeactivateBullet();
        break;

    default:
        break;
    }
}

void BulletManager::CheckBulletBulletCollisions(Bullet& currentBullet) {

    auto currentPosition = currentBullet.getPosition();

    for (auto& bullet : m_bullets) 
    {
        if (&currentBullet == &bullet || !bullet.IsActive()) {
            continue;
        }

        if (bullet.getPosition() == currentPosition) {
            currentBullet.DeactivateBullet();
            bullet.DeactivateBullet();
            m_gameMap.SetTile(currentPosition, TileType::EmptySpace);
            return; 
        }
    }
}


void BulletManager::CheckBulletPlayersCollisions(Bullet& bullet) {

    const auto bulletPosition = bullet.getPosition();
    const auto shooterID = bullet.GetShooterID();

    for (auto& player : m_players) {

        if (bulletPosition == player.getPosition() && shooterID != player.GetPlayerID()) {
            player.TakeDamage();
            player.respawn();
            bullet.DeactivateBullet();

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

void BulletManager::ProcessCollisions(Bullet& bullet)
{
    CheckBulletWallCollisions(bullet);

    if (!bullet.IsActive()) return;
    CheckBulletBulletCollisions(bullet);

    if (!bullet.IsActive()) return;
    CheckBulletPlayersCollisions(bullet);
}

