#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <functional>

#include "PlayerAction.h"


struct PlayerControls
{
	sf::Keyboard::Key moveLeft;
	sf::Keyboard::Key moveRight;
	sf::Keyboard::Key jump;
	sf::Keyboard::Key crouch;
};

class InputSystem
{
public:
	using ActionCallback = std::function<void(PlayerAction)>;

	explicit InputSystem(const PlayerControls& controls);

	void setActionCallback(ActionCallback callback);
	void update();

private:
	PlayerControls controls;
	ActionCallback actionCallback;

	void dispatchAction(PlayerAction action);


};

