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
		bool GetIsDestructible() const;
		bool GetIsDestroyed() const;
		bool GetHasBomb() const;
		std::pair<size_t, size_t> GetPosition() const;
		void SetPosition(const std::pair<int, int>& newPosition);
		void SetWallType(TileType newWallType);
		TileType GetWallType() const;

		int GetExplosionRadius() const;
};

