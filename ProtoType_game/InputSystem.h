#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include "PlayerControls.h"
#include "PlayerAction.h"
#include "InputContext.h"
#include <vector>


class InputSystem
{
public:
	using ActionCallback = std::function<void(PlayerAction)>;

	struct PlayerBinding
	{
		PlayerControls controls;
		ActionCallback actionCallback;
	};

	InputSystem();

	//void setActionCallback(ActionCallback callback);
	void update();
	void setContext(InputContext context);
	InputContext getContext() const;
	void addPlayer(const PlayerControls& controls, ActionCallback callback);

private:
	//PlayerControls controls;
	//ActionCallback actionCallback;

	InputContext currentContext;

	std::vector<PlayerBinding> players;

	void dispatchAction(PlayerBinding binding, PlayerAction action) const;
	

};

