#pragma once
#include "TileType.h"
#include<vector>
#include<array>
#include<cstdint>
#include<iostream>

class Map
{
	private:
		std::vector<std::vector<TileType>>m_gameArea;
		uint16_t m_height;
		uint16_t m_width;
		std::array<std::pair<uint16_t, uint16_t>,4>m_startPositions;
	public:
		uint16_t getHeight() const;
		void setHeight(uint16_t height);
		uint16_t getWidth() const;
		void setWidth(uint16_t width);
		TileType getTile(int x, int y) const;
		void destroyTile(int x, int y);
		void draw() const;
};