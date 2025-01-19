#include "Wall.h"

bool Wall::GetIsDestructible() const
{
	return isDestructible;
}

bool Wall::GetIsDestroyed() const
{
	return isDestroyed;
}

bool Wall::GetHasBomb() const
{
	return hasBomb;
}

std::pair<size_t, size_t> Wall::GetPosition() const
{
	return position;
}

void Wall::SetPosition(const std::pair<int, int>& newPosition)
{
	position = newPosition;
}

int Wall::GetExplosionRadius() const
{
	return explosionRadius;
}

void Wall::SetWallType(TileType newWallType)
{
     TileType wallType = newWallType;

    if (wallType == TileType::DestrucitbleWall) {
        isDestructible = true;
        isDestroyed = false;
        hasBomb = false;
    }
    else if (wallType == TileType::IndestrucitbleWall) {
        isDestructible = false;
        isDestroyed = false;
        hasBomb = false;
    }
    else if (wallType == TileType::DestrucitbleWallWithBomb) {
        isDestructible = true;
        isDestroyed = false;
        hasBomb = true;
    }
    else {
        isDestructible = false;
        isDestroyed = false;
        hasBomb = false;
    }
}

TileType Wall::GetWallType() const
{
    return wallType;
}
