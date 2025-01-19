#include "BulletManager.h"


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
                if (!HandlePortal(bullet, previousPosition)) {
                    bulletOpt.reset();
                    m_gameMap.SetTile(previousPosition, TileType::EmptySpace);
                    break;
                }
                else
                {
                    currentPosition = bullet.getPosition();
                }
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

    for (auto& bulletOpt : m_bullets | 
        std::views::filter([](const std::optional<Bullet>& bulletOpt) {
            return bulletOpt.has_value();
        }))
    {
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

    for (auto& player : m_players |
        std::views::filter([](const std::shared_ptr<Player>& player){
            return player!=nullptr;
    }))
    {
        if(!player->IsEliminated())
        {
            if (bulletPosition == player->getPosition() && shooterID != player->GetPlayerID()) {
                player->TakeDamage();
                if (player->GetHealth() > 0) {
                    player->Respawn(m_gameMap.GetStartPosition(player->GetPlayerID()));
                }
                else
                {
                    player->SetPlace(m_playersAlive--);
                }
                bulletOpt.reset();
                m_gameMap.SetTile(bulletPosition, TileType::EmptySpace);

                m_players[shooterID]->AddScore();
                return;
            }
        }
    }
}

bool BulletManager::CanShoot(const size_t& shooterID)
{
    auto now = std::chrono::steady_clock::now();
    if (m_lastShotTime.find(shooterID) == m_lastShotTime.end()) {
        return true;
    }
    return (now - m_lastShotTime[shooterID]) >= m_players[shooterID]->GetFireRate();
}

void BulletManager::ShootBullet(const std::pair<size_t, size_t>& position,const Direction& direction, const uint8_t shooterID,size_t speed)
{
    if(CanShoot(shooterID))
    {
        m_lastShotTime[shooterID] = std::chrono::steady_clock::now();
        std::optional<Bullet> newBullet = CreateEntity<Bullet>(position, direction, shooterID, speed);
        m_bullets.emplace_back(newBullet);
	}
}

BulletManager::BulletManager(Map& map, std::array < std::shared_ptr <Player>, 4>& players,int& playersAlive)
    : m_gameMap(map), m_players(players),m_playersAlive(playersAlive) {}

void BulletManager::ProcessCollisions(std::optional<Bullet>& bulletOpt)
{
    CheckBulletWallCollisions(bulletOpt);

    if (!bulletOpt) return;
    CheckBulletBulletCollisions(bulletOpt);

    if (!bulletOpt) return;
    CheckBulletPlayersCollisions(bulletOpt);
}

void BulletManager::BombExplosion(const std::pair<size_t, size_t>& bombPosition)
{
    const int radius = 5;
    for (auto [x, y] : std::views::iota(0, (radius * 2)* (radius * 2))
        | std::views::transform([bombPosition, radius](int offset) {
            return std::make_pair(
                bombPosition.first + (offset % (2 * radius)) - radius,
                bombPosition.second + (offset / (2 * radius)) - radius
            );
            }))
    {
        if (m_gameMap.InBounds({ x, y })) {
            auto tileType = m_gameMap.GetTile({ x, y });


            if (tileType == TileType::DestrucitbleWall || tileType == TileType::DestrucitbleWallWithBomb) {
                m_gameMap.SetTile({ x, y }, TileType::EmptySpace);

                if (tileType == TileType::DestrucitbleWallWithBomb) {
                    BombExplosion({ x, y });
                }
            }


            for (auto& player : m_players) {
					if(player!=nullptr)
					{
                        if (player->getPosition() == std::make_pair(x, y)) {
                            m_gameMap.SetTile({ x,y }, TileType::EmptySpace);
                            player->TakeDamage();
                            player->Respawn(m_gameMap.GetStartPosition(player->GetPlayerID()));
                            break;
                        }
					}
            }
        }
    }
}

bool BulletManager::HandlePortal(Bullet& bullet, const std::pair<size_t, size_t>& previousPosition)
{
    auto portal = m_gameMap.GetPortalByEntry(previousPosition);
    if (portal == nullptr) {
        return false;
    }

    auto [exitX, exitY] = portal->exit;
    if (m_gameMap.GetTile({ exitX,exitY }) !=TileType::EmptySpace){
        return false;
    }

    if (exitY == 0) bullet.SetDirection(Direction::Right);
    else if (exitY == m_gameMap.GetWidth() - 1) bullet.SetDirection(Direction::Left);
    else if (exitX == 0) bullet.SetDirection(Direction::Down);
    else if (exitX == m_gameMap.GetHeight() - 1) bullet.SetDirection(Direction::Up);

    bullet.SetPosition({ exitX,exitY });
    return true;
}






