#pragma once
#include "Player.h"

class PlayerController
{
private:
	Player& player;
public:
	PlayerController(Player& player);

	void handleAction(PlayerAction action);
};