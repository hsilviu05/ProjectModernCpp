#pragma once
#include "TileType.h"
#include<iostream>
class Wall
{
	private:
		bool isDestructible;
		bool isDestroyed;
		bool hasBomb;
		std::pair<int, int>position;
		int explosionRadius;
	public:
		bool getIsDestructible() const;
		bool getIsDestroyed() const;
		bool getHasBomb() const;
		std::pair<int, int> getPosition() const;
		void setPosition(const std::pair<int, int>& newPosition);
		void setWallType(TileType newWallType);

		int getExplosionRadius() const;
};

