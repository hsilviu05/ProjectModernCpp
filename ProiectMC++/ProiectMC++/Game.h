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
	void Start();
	Map GetMap() const;
	void ReceiveInput(const std::string& username, char input);
	void ProcessInput();


private:
	Map m_map;
	std::vector<Bullet> m_bullets;
	std::array<std::shared_ptr<Player>,4>m_players;
	std::vector<std::pair<std::string, char>> currentInputs; 
	std::unordered_map<std::string, size_t> userToPlayerID;
	std::vector<Wall> m_walls;
	void Update();
	void handleInput(const char& key, std::shared_ptr<Player> player, Map& gameMap, BulletManager& bulletManager);
};
