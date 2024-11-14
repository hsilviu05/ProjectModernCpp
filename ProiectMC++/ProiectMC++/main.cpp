#include <iostream>
#include "Map.h"
int main()
{
	Map gameMap;
	gameMap.GenerateMap();
	std::cout << gameMap.getHeight()<<"\n";
	std::cout << gameMap.getWidth()<<"\n";
	gameMap.Draw();
	return 0;
}