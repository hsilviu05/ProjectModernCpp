#pragma once
#include "TileType.h"
#include<vector>
#include<array>
#include<cstdint>
#include<iostream>
#include<random>

class Map
{
	private:
		std::vector<std::vector<TileType>>m_gameArea;
		size_t m_height;
		size_t m_width;
		void Allocation();
		std::array<std::pair<size_t, size_t>,4>m_startPositions;
	public:
		size_t getHeight() const;
		size_t getWidth() const;

		void GenerateMap();
		void SetStartPositions();
		void Draw() const;

		TileType getTile(int x, int y) const;
		void destroyTile(int x, int y);
};