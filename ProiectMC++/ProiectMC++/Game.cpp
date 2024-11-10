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
        if (bullet.checkBulletWallCollisions(m_walls,m_map))
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

void Game::render()
{

}

