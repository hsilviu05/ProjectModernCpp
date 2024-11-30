#pragma once
#include "TileType.h"
#include "Wall.h"
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
		std::array<std::pair<size_t, size_t>,4>m_playersPositions;
		std::array<std::pair<size_t, size_t>,4>m_startPositions;
		void Explode(const std::pair<size_t, size_t>& t_position);
		std::vector<Wall> walls;

	public:
		size_t getHeight() const;
		size_t getWidth() const;

		void GenerateMap();
		void SetStartPositions();
		std::pair<size_t,size_t> getStartPosition(const size_t& playerNumber) const;
		void Draw() const;

		std::vector<Wall>& GetWalls();

		TileType GetTile(const std::pair<size_t, size_t>&t_position) ;
		void DestroyTile(const std::pair<size_t, size_t>&t_position);
		void SetTile(const std::pair<size_t, size_t>& t_position,const TileType& t_tile);
		bool IsValidPosition(const std::pair<size_t,size_t>& position) const;

		bool inBounds(const std::pair<size_t,size_t>&position);


		std::pair<size_t,size_t> GetPlayerPosition(const size_t& playerNumber);
		void SetPlayerPosition(const size_t& playerNumber,const std::pair<size_t,size_t>& playerPosition);
};