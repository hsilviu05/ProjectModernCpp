#pragma once
#include <crow.h>
#include "Game.h"
#include "TileType.h"
#include <mutex>
class Routing
{
public:
	void run(Game& game);
private:
	std::mutex mapMutex;
};

