#include "Game.h"


Game::Game(PlayerManager& pm):m_map(),bulletManager(m_map,m_players)
{
    auto& activePlayers = pm.GetActivePlayers();
    for (size_t i = 0; i < activePlayers.size(); ++i)
    {
        m_players[i] = std::move(activePlayers[i]);

        m_players[i]->SetPosition(m_map.getStartPosition(i));
        m_players[i]->SetPlayerID(i);
        m_map.SetPlayerPosition(i, m_players[i]->getPosition());
        m_map.SetTile(m_players[i]->getPosition(), TileType::Player);
        m_players[i]->SetScore(0);
        m_players[i]->SetHealth(3);
    }
}

void Game::Start()
{
    while (true) {
        ProcessInput();
        Update();
        Sleep(200);
    }
}

void Game::Update()
{
    bulletManager.UpdateBullets();

    for (auto& player : m_players |
        std::views::filter([](const std::shared_ptr<Player>& player) {
            return player->IsAlive();
            }))
    {
        m_map.SetPlayerPosition(player->GetPlayerID(), player->getPosition());
        m_map.SetTile(player->getPosition(), TileType::Player);
    }
}

void Game::handleInput(const char& key, std::shared_ptr<Player> player, Map& gameMap, BulletManager& bulletManager)
{
        std::pair<size_t, size_t> newPosition = player->getPosition();
        switch (key) {
        case 'A':
            newPosition.second -= 1;
            player->SetDirection(Direction::Left);
            break;
        case 'D':
            newPosition.second += 1;
            player->SetDirection(Direction::Right);
            break;
        case 'W':
            newPosition.first -= 1;
            player->SetDirection(Direction::Up);
            break;
        case 'S':
            newPosition.first += 1;
            player->SetDirection(Direction::Down);
            break;
        case ' ':
            bulletManager.ShootBullet(player->getPosition(), player->GetDirection(), player->GetPlayerID(), GameSettings::DEFAULT_BULLET_SPEED);
            break;
        default:
            return;
        }

        if (gameMap.InBounds(newPosition) == true && gameMap.GetTile(newPosition) == TileType::EmptySpace) {
            gameMap.SetTile(player->getPosition(), TileType::EmptySpace);
            player->Move(key);
        }
        else if (!gameMap.InBounds(newPosition))
        {
            auto portal = gameMap.GetPortalByEntry(player->getPosition());
            if (portal != nullptr)
            {
                auto [exitX, exitY] = portal->exit;
                if (gameMap.GetTile({ exitX,exitY }) == TileType::EmptySpace)
                {
                    gameMap.SetTile(player->getPosition(), TileType::EmptySpace);
                    if (exitY == 0) {
                        player->SetDirection(Direction::Right);
                    }
                    else if (exitY == gameMap.GetWidth() - 1) {
                        player->SetDirection(Direction::Left);
                    }
                    else if (exitX == 0) {
                        player->SetDirection(Direction::Down);
                    }
                    else if (exitX == gameMap.GetHeight() - 1) {
                        player->SetDirection(Direction::Up);
                    }
                    player->SetPosition({ exitX,exitY });
                }
            }
        }
}



int Game::checkWinner() {
    int aliveCount = 0;
    int lastAliveIndex = -1;

    for (size_t i = 0; i < m_players.size(); ++i) {
        if (m_players[i]->IsAlive()) {
            aliveCount++;
            lastAliveIndex = static_cast<int>(i);
        }
    }

    if (aliveCount == 1) {
        return lastAliveIndex;
    }

    return -1;
}

Map Game::GetMap() const
{
    return m_map;
}

void Game::ReceiveInput(const std::string& username, char input)
{
    for (auto& player : m_players) {
        if (player->GetUsername() == username) {
            currentInputs.emplace_back(username, input);
            return;
        }
    }
}

void Game::ProcessInput()
{
    for (const auto& [username, input] : currentInputs) {
        // Găsește player-ul asociat cu username
        for (auto& player : m_players) {
            if (player && player->GetUsername() == username) { // Verifică player-ul și username-ul
                handleInput(input, player, m_map, bulletManager);
                break; 
            }
        }
    }
    currentInputs.clear();
}

