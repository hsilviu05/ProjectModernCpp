#include "Game.h"


Game::Game()
{

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
    //char input;
    //std::cout << "Use W, A, S, D to move: ";
    //std::cin >> input;

    //switch (input) {
    //case 'w': case 'W':
    //    player.move(Direction::Up);
    //    break;
    //case 'a': case 'A':
    //    player.move(Direction::Left);
    //    break;
    //case 's': case 'S':
    //    player.move(Direction::Down);
    //    break;
    //case 'd': case 'D':
    //    player.move(Direction::Right);
    //    break;
    //default:
    //    std::cout << "Invalid input. Use W, A, S, D.\n";
    //}
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

