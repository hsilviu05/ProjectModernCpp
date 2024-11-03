#include "Wall.h"

bool Wall::getIsDestructible() const
{
	return isDestructible;
}

bool Wall::getIsDestroyed() const
{
	return isDestroyed;
}

std::pair<int, int> Wall::getPosition() const
{
	return position;
}

void Wall::setPosition(const std::pair<int, int>& newPosition)
{
	position = newPosition;
}
