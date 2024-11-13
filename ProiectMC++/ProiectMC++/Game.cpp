#include "Game.h"

Game::Game()
{

}

void Game::start()
{
}

void Game::update()
{
    for (auto& bullet : m_bullets) 
    {
        //bullet.moveBullet();
        if (bullet.CheckBulletWallCollisions(m_walls,m_map))
        {
            continue;
        }
    }
}

void Game::handleInput(Player& player)
{
    char input;
    std::cout << "Use W, A, S, D to move: ";
    std::cin >> input;

    switch (input) {
    case 'w': case 'W':
        player.move(Direction::Up);
        break;
    case 'a': case 'A':
        player.move(Direction::Left);
        break;
    case 's': case 'S':
        player.move(Direction::Down);
        break;
    case 'd': case 'D':
        player.move(Direction::Right);
        break;
    default:
        std::cout << "Invalid input. Use W, A, S, D.\n";
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
                player.takeDamage();
                std::cout << "Player at position " << playerPosition.first << ", " << playerPosition.second
                    << " took damage from explosion!" << std::endl;
            }
        }
    }
}


void Game::render()
{

}

