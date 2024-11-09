#pragma once
#include<iostream>
class BulletPosition
{
private:
	std::pair<int, int>b_position;

public:
	BulletPosition(int x_cord, int y_cord);

	std::pair<int, int> getPosition() const;

	void SetPosition(int x_cord, int y_cord);

	bool operator==(const BulletPosition& other) const;
};