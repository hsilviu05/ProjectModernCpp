#pragma once
#include <crow.h>
#include "Game.h"
#include "TileType.h"
#include "PlayerManager.h"
#include <mutex>
#include "..\LoggingDLL\AccountManager.h"
class Routing
{
public:
	void Run();
	void StartGame();
	void ResetGame();
private:
	std::mutex mapMutex;
	PlayerManager playerManager;
	AccountManager accountManager;
	std::unique_ptr<Game> game;
};

