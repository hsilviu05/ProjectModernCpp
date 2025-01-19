import Direction;
#include <iostream>
#include "Map.h"
#include "Bullet.h"
#include "Player.h"
#include <crow.h>
#include <Windows.h>
#include <sqlite_orm/sqlite_orm.h>
#include "Game.h"
#include"TileType.h"
#include "Routing.h"

#include "BulletManager.h"

#include "..\LoggingDLL\AccountManager.h"

using namespace sqlite_orm;




int main()
{
    Routing routing;
    routing.Run();
}