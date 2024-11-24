#pragma once
#include<cstdint>

enum class TileType :uint8_t
{
	EmptySpace,
	DestrucitbleWall,
	IndestrucitbleWall,
	DestrucitbleWallWithBomb,
	Player
};