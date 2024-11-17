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


TileType Map::GetTile(const std::pair<size_t,size_t>&t_position) const
{
	if (t_position.first >= 0 && t_position.first < m_height && t_position.second >= 0 && t_position.second < m_width) {
		return m_gameArea[t_position.first][t_position.second];
	}
	
}

void Map::DestroyTile(const std::pair<size_t, size_t>&t_position)
{
	if (t_position.first >= 0 && t_position.first < m_height && t_position.second >= 0 && t_position.second < m_width) {
		if (m_gameArea[t_position.first][t_position.second] == TileType::DestrucitbleWall) {
			m_gameArea[t_position.first][t_position.second] = TileType::EmptySpace;
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

void Map::SetTile(const std::pair<size_t, size_t>& t_position,const TileType& t_tile){
	if (t_position.first >= 0 && t_position.first < m_height && t_position.second >= 0 && t_position.second < m_width){
		m_gameArea[t_position.first][t_position.second] = t_tile;
	}
}
