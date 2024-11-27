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

	std::uniform_int_distribution<size_t> dist_size(23, 25);
	m_height = dist_size(gen);
	m_width = dist_size(gen);

	std::discrete_distribution<int> dist_tile({ 6, 3, 1 });
	Allocation();

	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			if ((i == 0 || i == m_height - 1) && (j == 0 || j == m_width - 1))
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
	//generam automat  nr de ziduri intre 1-3
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

void Map::SetStartPositions() {
	m_startPositions = { {{0, 0}, { 0,m_width - 1 }, { m_height - 1,m_width - 1 }, { m_height - 1,0 } } };
}

std::pair<size_t, size_t> Map::getStartPosition(const size_t& playerNumber) const
{
	return m_startPositions[playerNumber];
}


size_t Map::getWidth() const{
	return m_width;
}


TileType Map::GetTile(const std::pair<size_t,size_t>&t_position)
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
		else if (m_gameArea[t_position.first][t_position.second] == TileType::DestrucitbleWallWithBomb) {
			m_gameArea[t_position.first][t_position.second] = TileType::EmptySpace; // Distruge zidul
			Explode(t_position); // Apelează funcția pentru explozie
		}
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
	if (t_position.first >= 0 && t_position.first < m_height && t_position.second >= 0 && t_position.second < m_width){
		m_gameArea[t_position.first][t_position.second] = t_tile;
	}
}

void Map::Explode(const std::pair<size_t, size_t>& t_position)
{
	size_t startRow = (t_position.first >= 3) ? t_position.first - 3 : 0;
	size_t endRow = std::min(t_position.first + 3, m_height - 1);

	size_t startCol = (t_position.second >= 3) ? t_position.second - 3 : 0;
	size_t endCol = std::min(t_position.second + 3, m_width - 1);

	for (size_t i = startRow; i <= endRow; ++i) {
		for (size_t j = startCol; j <= endCol; ++j) {
			if (m_gameArea[i][j] == TileType::DestrucitbleWall || m_gameArea[i][j] == TileType::DestrucitbleWallWithBomb) {
				m_gameArea[i][j] = TileType::EmptySpace; //distrugem zidurile pe o raza de 3
			}
		}
	}
}


