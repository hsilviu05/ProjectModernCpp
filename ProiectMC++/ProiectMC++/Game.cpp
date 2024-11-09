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
        bullet.moveBullet();
        if (bullet.checkBulletWallCollisions(m_walls,m_map))
        {
            continue;
        }
    }
}

void Game::handleInput()
{
}

void Game::render()
{

}

