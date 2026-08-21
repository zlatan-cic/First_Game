#include "PlayerController.h"


PlayerController::PlayerController(Player& player) : player(player)
{
}

void PlayerController::handleAction(PlayerAction action)
{
	player.handleAction(action);
}

