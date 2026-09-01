#pragma once
#include "Player.h"
#include "PlayerAction.h"


class PlayerController
{
private:
	Player& player;


public:
	PlayerController(Player& player);

	void handleAction(PlayerAction action);
};