#pragma once
#include "Player.h"
#include <vector>
#include <array>
#include <memory>
class PlayerManager
{
public:
    PlayerManager() = default;
    std::array<std::shared_ptr<Player>, 4>& GetActivePlayers();
    void AddToLobby(std::shared_ptr<Player> player);
    const std::vector<std::shared_ptr<Player>>& GetLobbyPlayersConst() const;
    std::vector<std::shared_ptr<Player>>& GetLobbyPlayers();

private:
    std::vector<std::shared_ptr<Player>> m_lobbyPlayers;   
    std::array<std::shared_ptr<Player>,4>m_activePlayers; 
};

