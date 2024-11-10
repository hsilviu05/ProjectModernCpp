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

std::pair<int, int> Wall::getPosition() const
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
