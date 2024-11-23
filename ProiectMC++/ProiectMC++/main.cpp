#include <iostream>
#include "Map.h"
#include "Bullet.h"
#include "Direction.h"

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

	return 0;
}