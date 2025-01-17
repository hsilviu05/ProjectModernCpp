#include "PlayerManager.h"

std::array<std::shared_ptr<Player>, 4>& PlayerManager::GetActivePlayers()
{
	return m_activePlayers;
}

void PlayerManager::AddToLobby(std::shared_ptr<Player> player)
{
	m_lobbyPlayers.push_back(player);
}

const std::vector<std::shared_ptr<Player>>& PlayerManager::GetLobbyPlayers() const {
	return m_lobbyPlayers;
}



