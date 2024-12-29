#include "Game.h"

#include "GameDatabase.h"


Game::Game()
{

}

void Game::start()
{
    gameMap.GenerateMap();
    bulletManager(m_map, m_players);
    player.setPosition(gameMap.getStartPosition(0));
    player.SetPlayerID(0);
    player2.setPosition(gameMap.getStartPosition(1));
    player2.SetPlayerID(1);

    gameMap.SetPlayerPosition(0, player.getPosition());
    gameMap.SetTile(player.getPosition(), TileType::Player);
    gameMap.SetPlayerPosition(1, player2.getPosition());
    gameMap.SetTile(player2.getPosition(), TileType::Player);
    gameMap.Draw();

    while (true) {
        update();
        render();
        Sleep(200); // Controlul vitezei jocului
    }
}

void Game::update()
{
    gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::EmptySpace);

    handleInput('A', player, gameMap, bulletManager);
    handleInput('D', player, gameMap, bulletManager);
    handleInput('W', player, gameMap, bulletManager);
    handleInput('S', player, gameMap, bulletManager);
    handleInput(VK_SPACE, player, gameMap, bulletManager);

    handleInputForPlayer2(VK_LEFT, player2, gameMap, bulletManager);
    handleInputForPlayer2(VK_RIGHT, player2, gameMap, bulletManager);
    handleInputForPlayer2(VK_UP, player2, gameMap, bulletManager);
    handleInputForPlayer2(VK_DOWN, player2, gameMap, bulletManager);
    handleInputForPlayer2('M', player2, gameMap, bulletManager);

    std::array<Player, 4> players = { player, player2 };
    bulletManager.UpdateBullets();

    gameMap.SetPlayerPosition(0, player.getPosition());
    gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::Player);
    gameMap.SetPlayerPosition(1, player2.getPosition());
    gameMap.SetTile(gameMap.GetPlayerPosition(1), TileType::Player);

}

void Game::handleInput(const char& key, Player& player, Map& gameMap, BulletManager& bulletManager)
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
            bulletManager.ShootBullet(player.getPosition(), player.getDirection(), player.GetPlayerID(), GameSettings::DEFAULT_BULLET_SPEED);
            break;
        default:
            return;
        }

        if (gameMap.InBounds(newPosition) == true && gameMap.GetTile(newPosition) == TileType::EmptySpace) {
            player.move(key);
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
            player2.setDirection(Direction::Left);
            break;
        case VK_RIGHT:
            newPosition2.second += 1;
            player2.setDirection(Direction::Right);
            break;
        case VK_UP:
            newPosition2.first -= 1;
            player2.setDirection(Direction::Up);
            break;
        case VK_DOWN:
            newPosition2.first += 1;
            player2.setDirection(Direction::Down);
            break;
        case 'M':
            bulletManager.ShootBullet(player2.getPosition(), player2.getDirection(), player2.GetPlayerID(), GameSettings::DEFAULT_BULLET_SPEED);
            break;
        default:
            return;
        }

        if (gameMap.InBounds(newPosition2) == true && gameMap.GetTile(newPosition2) == TileType::EmptySpace) {
            player2.move(key);
        }
    }
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
    gameMap.Draw();
}

