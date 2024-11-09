#include "Map.h"

uint16_t Map::getHeight() const{
	return m_height;
}

void Map::setHeight(uint16_t height){
	m_height = height;
}

uint16_t Map::getWidth() const{
	return m_width;
}

void Map::setWidth(uint16_t width){
	m_width = width;
}

TileType Map::getTile(int x, int y) const
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
		return m_gameArea[y][x];
	}
	return TileType::EmptySpace;
}

void Map::destroyTile(int x, int y)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
		if (m_gameArea[y][x] == TileType::DestrucitbleWall) {
			m_gameArea[y][x] = TileType::EmptySpace;
		}
	}
}
