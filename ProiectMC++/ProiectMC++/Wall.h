#pragma once
#include "TileType.h"
#include<iostream>
class Wall
{
	private:
		bool isDestructible;
		bool isDestroyed;
		bool hasBomb;
		std::pair<size_t, size_t>position;
		int explosionRadius;
		TileType wallType;

	public:
		bool getIsDestructible() const;
		bool getIsDestroyed() const;
		bool getHasBomb() const;
		std::pair<size_t, size_t> getPosition() const;
		void setPosition(const std::pair<int, int>& newPosition);
		void setWallType(TileType newWallType);
		TileType getWallType() const;

		int getExplosionRadius() const;
};

