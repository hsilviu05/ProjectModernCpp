#pragma once
#include "Player.h"
#include <vector>
#include <array>
#include <memory>
class PlayerManager
{
public:
    std::array<std::shared_ptr<Player>, 4>& GetActivePlayers();
private:
    PlayerManager() = default;
    std::vector<std::shared_ptr<Player>> m_lobbyPlayers;   
    std::array<std::shared_ptr<Player>,4>m_activePlayers; 
};

