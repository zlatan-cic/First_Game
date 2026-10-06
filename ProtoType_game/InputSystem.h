#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include "PlayerControls.h"
#include "PlayerAction.h"
#include "InputContext.h"


class InputSystem
{
public:
	using ActionCallback = std::function<void(PlayerAction)>;

	explicit InputSystem(const PlayerControls& controls);

	void setActionCallback(ActionCallback callback);
	void update();
	void setContext(InputContext context);
	InputContext getContext() const;

private:
	PlayerControls controls;
	ActionCallback actionCallback;

	InputContext currentContext;

	void dispatchAction(PlayerAction action);


	


};

