#pragma once

#include "Bullet.h"
#include "Player.h"
#include "Map.h"
#include "Wall.h"
#include "BulletManager.h"
#include <windows.h>
#include "PlayerManager.h"
#include <memory>
#include <queue>

class Game
{
private:
	Player player;
	BulletManager bulletManager;
	int checkWinner();
	bool m_isGameOver = false;

public:
	Game();
	Game(PlayerManager& pm);
	void start();
	void update();
	void handleInput(const char& key, std::shared_ptr<Player> player, Map& gameMap, BulletManager& bulletManager);
	void render();

private:
	Map m_map;
	std::vector<Bullet> m_bullets;
	std::array<std::shared_ptr<Player>,4>m_players;
	std::unordered_map<int, std::queue<char>> playerInputs;
	std::vector<Wall> m_walls;

};
