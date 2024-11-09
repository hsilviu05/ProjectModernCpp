#include "BulletPosition.h"

BulletPosition::BulletPosition(int x_cord, int y_cord)
	: b_position(x_cord, y_cord) {}

std::pair<int, int> BulletPosition::getPosition() const
{
	return b_position;
}

void BulletPosition::SetPosition(int x_cord, int y_cord)
{
	b_position = std::make_pair(x_cord, y_cord);
}

bool BulletPosition::operator==(const BulletPosition& other) const
{
	return b_position == other.b_position ;
}
