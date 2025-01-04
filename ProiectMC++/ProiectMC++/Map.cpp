#include "Map.h"


void Map::Allocation(){
	m_gameArea.resize(m_height, std::vector<TileType>(m_width));
}

Map::Map()
{
	GenerateMap();
}

size_t Map::getHeight() const{
	return m_height;
}

void Map::GenerateMap(){
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<size_t> dist_size(23, 25);
	m_height = dist_size(gen);
	m_width = dist_size(gen);

	std::discrete_distribution<int> dist_tile({ 6, 3, 1 });
	Allocation();

	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			if (i == 0 || i == m_height - 1 || j == 0 || j == m_width - 1)
				m_gameArea[i][j] = TileType::EmptySpace;
			else {
				TileType tile = static_cast<TileType>(dist_tile(gen));
				m_gameArea[i][j] = tile;

				if (tile == TileType::DestrucitbleWall || tile == TileType::IndestrucitbleWall) {
					Wall wall;
					wall.setPosition({ i, j });
					wall.setWallType(tile);
					walls.push_back(wall);
				}
			}
		}
	}
	std::uniform_int_distribution<int> dist_bombs(1, 3);
	int bombWallsCount = dist_bombs(gen);

	std::uniform_int_distribution<size_t> dist_x(1, m_height - 2);
	std::uniform_int_distribution<size_t> dist_y(1, m_width - 2);

	int bombWallsPlaced = 0;
	while (bombWallsPlaced < bombWallsCount) {
		size_t x = dist_x(gen);
		size_t y = dist_y(gen);

		if (m_gameArea[x][y] == TileType::DestrucitbleWall) {
			m_gameArea[x][y] = TileType::DestrucitbleWallWithBomb;
			bombWallsPlaced++;
		}
	}
}

std::pair<size_t, size_t> Map::getStartPosition(const size_t& playerID) const
{
	switch (playerID) {
	case 0: return { 0, 0 };                            
	case 1: return { 0, m_width - 1 };                  
	case 2: return { m_height - 1, m_width - 1 };       
	case 3: return { m_height - 1, 0 };                 
	default: throw std::out_of_range("Invalid playerID");
	}
}


size_t Map::getWidth() const{
	return m_width;
}


TileType Map::GetTile(const std::pair<size_t,size_t>& t_position)
{
	if (InBounds(t_position)) {
		return m_gameArea[t_position.first][t_position.second];
	}
}

void Map::Draw() const
{
	system("cls");
	for (const auto& row : m_gameArea) {
		for (const auto& tile : row){
			std::cout << static_cast<int>(tile) << " ";
		}
		std::cout << "\n";
	}
}

std::vector<Wall>& Map::GetWalls()
{
	return walls;
}


std::pair<size_t, size_t> Map::GetPlayerPosition(const size_t& playerNumber)
{
	return m_playersPositions[playerNumber];
}

void Map::SetPlayerPosition(const size_t& playerNumber, const std::pair<size_t, size_t>& playerPosition)
{
	m_playersPositions[playerNumber] = playerPosition;
}


void Map::SetTile(const std::pair<size_t, size_t>& t_position,const TileType& t_tile){
	if (InBounds(t_position)){
		m_gameArea[t_position.first][t_position.second] = t_tile;
	}
}

bool Map::InBounds(const std::pair<size_t, size_t>& position)
{
	return position.first <= m_height-1 && position.second <= m_width-1;
}


