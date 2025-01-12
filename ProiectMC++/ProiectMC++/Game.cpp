#include "Game.h"



Game::Game():m_map(),bulletManager(m_map,m_players)
{

}

void Game::start()
{
    //gameMap.GenerateMap();
    player.SetPosition(m_map.getStartPosition(0));
    player.SetPlayerID(0);

    m_map.SetPlayerPosition(0, player.getPosition());
    m_map.SetTile(player.getPosition(), TileType::Player);
    m_map.SetPlayerPosition(1, player2.getPosition());
    m_map.SetTile(player2.getPosition(), TileType::Player);
    m_map.Draw();

    BulletManager bulletManager(m_map, m_players);

    while (true) {
        update();
        render();
        Sleep(200); // Controlul vitezei jocului
    }
}

void Game::update()
{
   //m_map.SetTile(m_map.GetPlayerPosition(0), TileType::EmptySpace);

    handleInput('A', player, m_map, bulletManager);
    handleInput('D', player, m_map, bulletManager);
    handleInput('W', player, m_map, bulletManager);
    handleInput('S', player, m_map, bulletManager);
    handleInput(VK_SPACE, player, m_map, bulletManager);

    std::array<Player, 4> players = { player, player2 };
    bulletManager.UpdateBullets();

    m_map.SetPlayerPosition(0, player.getPosition());
    m_map.SetTile(m_map.GetPlayerPosition(0), TileType::Player);

   }

void Game::handleInput(const char& key, Player& player, Map& gameMap, BulletManager& bulletManager)
{
    if (GetAsyncKeyState(key) & 0x8000) {
        std::pair<size_t, size_t> newPosition = player.getPosition();
        switch (key) {
        case 'A':
            newPosition.second -= 1;
            player.SetDirection(Direction::Left);
            break;
        case 'D':
            newPosition.second += 1;
            player.SetDirection(Direction::Right);
            break;
        case 'W':
            newPosition.first -= 1;
            player.SetDirection(Direction::Up);
            break;
        case 'S':
            newPosition.first += 1;
            player.SetDirection(Direction::Down);
            break;
        case VK_SPACE:
            bulletManager.ShootBullet(player.getPosition(), player.GetDirection(), player.GetPlayerID(), GameSettings::DEFAULT_BULLET_SPEED);
            break;
        default:
            return;
        }

        if (gameMap.InBounds(newPosition) == true && gameMap.GetTile(newPosition) == TileType::EmptySpace) {
            gameMap.SetTile(player.getPosition(), TileType::EmptySpace);
            player.Move(key);
        }
        else if (!gameMap.InBounds(newPosition))
        {
            auto portal = gameMap.GetPortalByEntry(player.getPosition());
            if (portal != nullptr)
            {
                auto [exitX, exitY] = portal->exit;
                if (gameMap.GetTile({ exitX,exitY }) == TileType::EmptySpace)
                {
                    gameMap.SetTile(player.getPosition(), TileType::EmptySpace);
                    if (exitY == 0) {
                        player.SetDirection(Direction::Right);
                    }
                    else if (exitY == gameMap.GetWidth() - 1) {
                        player.SetDirection(Direction::Left);
                    }
                    else if (exitX == 0) {
                        player.SetDirection(Direction::Down);
                    }
                    else if (exitX == gameMap.GetHeight() - 1) {
                        player.SetDirection(Direction::Up);
                    }
                    player.SetPosition({ exitX,exitY });
                }
            }
        }
    }
}

void Game::handleInputForPlayer2(const char& key, Player& player2, Map& gameMap, BulletManager& bulletManager)
{
    if (GetAsyncKeyState(key) & 0x8000) {
        std::pair<size_t, size_t> newPosition2 = player2.getPosition();
        switch (key) {
        case VK_LEFT:
            newPosition2.second -= 1;
            player2.SetDirection(Direction::Left);
            break;
        case VK_RIGHT:
            newPosition2.second += 1;
            player2.SetDirection(Direction::Right);
            break;
        case VK_UP:
            newPosition2.first -= 1;
            player2.SetDirection(Direction::Up);
            break;
        case VK_DOWN:
            newPosition2.first += 1;
            player2.SetDirection(Direction::Down);
            break;
        case 'M':
            bulletManager.ShootBullet(player2.getPosition(), player2.GetDirection(), player2.GetPlayerID(), GameSettings::DEFAULT_BULLET_SPEED);
            break;
        default:
            return;
        }

        if (gameMap.InBounds(newPosition2) == true && gameMap.GetTile(newPosition2) == TileType::EmptySpace) {
            player2.Move(key);
        }
    }
}

int Game::checkWinner() {
    int aliveCount = 0;
    int lastAliveIndex = -1;

    for (size_t i = 0; i < m_players.size(); ++i) {
        if (m_players[i].IsAlive()) {
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
    m_map.Draw();
}

