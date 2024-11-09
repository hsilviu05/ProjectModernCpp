#pragma once

#include "Bullet.h"
#include "Player.h"
#include "Map.h"
#include "Wall.h"
#include "Level.h"

class Game
{
public:
	Game();
	void start();
	void update();
	void handleInput();
	void render();

private:
	Map m_map;
	std::vector<Bullet> m_bullets;
	std::vector<Wall> m_walls;

};