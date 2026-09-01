#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include "PlayerControls.h"
#include "PlayerAction.h"


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

