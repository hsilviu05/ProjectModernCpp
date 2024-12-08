#include "Game.h"


Game::Game()
{
    gameMap.GenerateMap();
    player.setPosition(gameMap.getStartPosition(0));
    player.SetPlayerID(0);
    gameMap.SetPlayerPosition(0, player.getPosition());
    gameMap.SetTile(player.getPosition(), TileType::Player);
    gameMap.Draw();
}

void Game::start()
{
}

void Game::update()
{
    /*for (auto& bullet : m_bullets)
    {
        bullet.MoveBullet();

        if (bullet.IsActive())
        {
            bullet.CheckBulletWallCollisions(m_walls, m_map);
            if (!bullet.IsActive()) 
                continue;
            bullet.CheckBulletBulletCollisions(m_bullets);
            if (!bullet.IsActive()) 
                continue;
            bullet.CheckBulletPlayersCollisions(m_players);
        }
    }*/

    int winnerIndex = checkWinner();
    if (winnerIndex != -1) {
        m_players[winnerIndex].AddWinBonus();
        m_players[winnerIndex].AddScore(2);
        m_isGameOver = true;
        //trb adaugat si pentru second place
    }

}

void Game::handleInput(Player& player)
{
    /*
    void handleInput(const char& key, Player & player, Map & gameMap, BulletManager & bulletManager)
    {
        if (GetAsyncKeyState(key) & 0x8000) {
            std::pair<size_t, size_t> newPosition = player.getPosition();
            switch (key) {
            case 'A':
                newPosition.second -= 1;
                player.setDirection(Direction::Left);
                break;
            case 'D':
                newPosition.second += 1;
                player.setDirection(Direction::Right);
                break;
            case 'W':
                newPosition.first -= 1;
                player.setDirection(Direction::Up);
                break;
            case 'S':
                newPosition.first += 1;
                player.setDirection(Direction::Down);
                break;
            case VK_SPACE:
                bulletManager.AddBullet(player.shoot());

                break;
            default:
                return;
            }

            if (gameMap.inBounds(newPosition) == true && gameMap.GetTile(newPosition) == TileType::EmptySpace) {
                player.move(key);
            }
        }
    }
    */
}

void Game::handleExplosion(Wall& wall) {

    if (wall.getHasBomb()) {
        int explosionRadius = wall.getExplosionRadius();
        std::pair<int, int> wallPosition = wall.getPosition();

        for (auto& player : m_players) {
            std::pair<int, int> playerPosition = player.getPosition();

            int distance = std::abs(wallPosition.first - playerPosition.first) +
                std::abs(wallPosition.second - playerPosition.second);

            if (distance <= explosionRadius) {
                player.TakeDamage();
                std::cout << "Player at position " << playerPosition.first << ", " << playerPosition.second
                    << " took damage from explosion!" << std::endl;
            }
        }
    }
}

int Game::checkWinner() {
    int aliveCount = 0;
    int lastAliveIndex = -1;

    for (size_t i = 0; i < m_players.size(); ++i) {
        if (m_players[i].isAlive()) {
            aliveCount++;
            lastAliveIndex = static_cast<int>(i);
        }
    }

    if (aliveCount == 1) {
        return lastAliveIndex;
    }

    return -1;
}




void Game::render()
{

}

