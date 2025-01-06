#pragma once
#include "TileType.h"
#include "Wall.h"
#include<vector>
#include<array>
#include<cstdint>
#include<iostream>
#include<random>
#include <unordered_set>

struct Portal {
	std::pair<size_t, size_t> entry;
	std::pair<size_t, size_t> exit;

	bool operator==(const Portal& other) const {
		return (entry == other.entry && exit == other.exit) ||
			(entry == other.exit && exit == other.entry);
	}
};

struct PortalHash {
	std::size_t operator()(const Portal& p) const {
		auto hash1 = std::hash<size_t>{}(p.entry.first) ^ (std::hash<size_t>{}(p.entry.second) << 1);
		auto hash2 = std::hash<size_t>{}(p.exit.first) ^ (std::hash<size_t>{}(p.exit.second) << 1);
		return hash1 ^ hash2;
	}
};

class Map
{
	private:
		std::vector<std::vector<TileType>>m_gameArea;
		size_t m_height;
		size_t m_width;
		std::unordered_set<Portal,PortalHash>m_portals;
		void Allocation();
		std::array<std::pair<size_t, size_t>,4>m_playersPositions;
		std::vector<Wall> walls;
		void GeneratePortals(std::mt19937& gen);
		std::pair<size_t, size_t> GeneratePortalPart(std::mt19937& gen, std::bernoulli_distribution& bernoulli,const bool& part);

	public:
		Map();
		size_t getHeight() const;
		size_t getWidth() const;

		void GenerateMap();

		std::pair<size_t,size_t> getStartPosition(const size_t& playerID) const;

		void Draw() const;

		std::vector<Wall>& GetWalls();

		TileType GetTile(const std::pair<size_t, size_t>&t_position) ;
		void SetTile(const std::pair<size_t, size_t>& t_position,const TileType& t_tile);

		bool InBounds(const std::pair<size_t,size_t>&position);

		std::pair<size_t,size_t> GetPlayerPosition(const size_t& playerNumber);
		void SetPlayerPosition(const size_t& playerNumber,const std::pair<size_t,size_t>& playerPosition);
};