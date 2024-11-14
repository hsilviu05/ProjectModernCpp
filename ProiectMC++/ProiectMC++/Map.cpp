#include "Map.h"


void Map::Allocation(){
	m_gameArea.resize(m_height);
	for (int i = 0; i < m_height; i++)
		m_gameArea[i].resize(m_width);
}

size_t Map::getHeight() const{
	return m_height;
}

void Map::GenerateMap(){

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<size_t> dist_size(12, 25);
	m_height = dist_size(gen);
	m_width = dist_size(gen);

	std::uniform_int_distribution<uint16_t> dist_tile(0, 2);
	Allocation();
	for(int i=0;i<m_height;i++){
		for (int j = 0; j < m_width; j++)
			if ((i == 0 || i == m_height-1) && (j == 0 || j == m_width-1))
				m_gameArea[i][j] = TileType::EmptySpace;
			else
				m_gameArea[i][j] = static_cast<TileType>(dist_tile(gen));
	}
	

}

void Map::SetStartPositions() {
	m_startPositions = { {{0, 0}, { 0,m_width - 1 }, { m_height - 1,m_width - 1 }, { m_height - 1,0 } } };
}

size_t Map::getWidth() const{
	return m_width;
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

void Map::Draw() const
{
	for (const auto& row : m_gameArea) {
		for (const auto& tile : row){
			std::cout << static_cast<int>(tile) << " ";
		}
		std::cout << "\n";
	}
}
