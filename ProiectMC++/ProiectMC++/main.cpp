#include <iostream>
#include "Map.h"
#include "Bullet.h"
#include "Direction.h"
#include "Player.h"
#include <Windows.h>



char getPressedKey() {
	if (GetAsyncKeyState('W') & 0x8000) return 'W';
	if (GetAsyncKeyState('S') & 0x8000) return 'S';
	if (GetAsyncKeyState('A') & 0x8000) return 'A';
	if (GetAsyncKeyState('D') & 0x8000) return 'D';
	return 0; 
}

int main()
{
	Map gameMap;
	gameMap.GenerateMap();
	std::cout << "Harta generata:\n";
	gameMap.Draw();
	std::pair<size_t, size_t> bombPosition{ 3, 3 };
	gameMap.SetTile(bombPosition, TileType::DestrucitbleWallWithBomb);
	std::cout << "Harta generata cu bomba :\n";
	gameMap.Draw();
	gameMap.DestroyTile(bombPosition);
	std::cout << "Harta generata cu bomba dupa explozie:\n";
	gameMap.Draw();

	Bullet::Position initialPosition(5, 5);
	Direction bulletDirection = Direction::Left;

	Bullet bullet(initialPosition, bulletDirection);

	auto pos = bullet.GetBulletPosition().getPosition();
	std::cout << "Pozitia initiala a glontului: (" << pos.first << ", " << pos.second << ")\n"; 	
	bullet.MoveBullet();

	pos = bullet.GetBulletPosition().getPosition();
	std::cout << "Pozitia glontului dupa miscare: (" << pos.first << ", " << pos.second << ")\n";

	Player player;
	player.setPosition(gameMap.getStartPosition(0));
	gameMap.SetPlayerPosition(0,player.getPosition());
	gameMap.SetTile(player.getPosition(), TileType::Player);
	while(true)
	{
		gameMap.SetTile(gameMap.GetPlayerPosition(0), TileType::EmptySpace);
		switch (getPressedKey()) {
		case 'W':
			if (player.getPosition().first > 0)
				player.move('W');
			break;
		case 'S':
			if (player.getPosition().first < gameMap.getHeight()-1)
				player.move('S');
			break;
		case 'A':
			if (player.getPosition().second > 0)
				player.move('A');
			break;
		case 'D':
			if (player.getPosition().second < gameMap.getWidth()-1)
				player.move('D');
			break;
		default:
			break;
		}
	}

	return 0;
}