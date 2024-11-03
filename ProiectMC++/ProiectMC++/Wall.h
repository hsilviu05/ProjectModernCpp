#pragma once
#include<iostream>
class Wall
{
	private:
		bool isDestructible;
		bool isDestroyed;
		std::pair<int, int>position;
	public:
		bool getIsDestructible() const;
		bool getIsDestroyed() const;
		std::pair<int, int> getPosition() const;
		void setPosition(const std::pair<int, int>& newPosition);
};

