#include "Wall.h"

bool Wall::getIsDestructible() const
{
	return isDestructible;
}

bool Wall::getIsDestroyed() const
{
	return isDestroyed;
}

bool Wall::getHasBomb() const
{
	return hasBomb;
}

std::pair<size_t, size_t> Wall::getPosition() const
{
	return position;
}

void Wall::setPosition(const std::pair<int, int>& newPosition)
{
	position = newPosition;
}

int Wall::getExplosionRadius() const
{
	return explosionRadius;
}

void Wall::setWallType(TileType newWallType)
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

TileType Wall::getWallType() const
{
    return wallType;
}
