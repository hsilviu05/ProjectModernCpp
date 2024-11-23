#include <iostream>
#include "Map.h"
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
	return 0;
}