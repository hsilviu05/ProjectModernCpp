#include "PlayerManager.h"

std::array<std::shared_ptr<Player>, 4>& PlayerManager::GetActivePlayers()
{
	return m_activePlayers;
}
