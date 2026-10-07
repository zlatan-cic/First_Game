#include "InputSystem.h"

#include <utility>

InputSystem::InputSystem() : currentContext(InputContext::Gameplay)
{

}

//void InputSystem::setActionCallback(ActionCallback callback)
//{
//	actionCallback = std::move(callback);
//}

void InputSystem::update()
{
	if (currentContext != InputContext::Gameplay)
	{
		return;
	}
	for (const auto& binding : players)
	{

		if (sf::Keyboard::isKeyPressed(binding.controls.moveLeft))
		{
			dispatchAction(binding, PlayerAction::MoveLeft);
		}

		if (sf::Keyboard::isKeyPressed(binding.controls.moveRight))
		{
			dispatchAction(binding, PlayerAction::MoveRight);
		}

		if (sf::Keyboard::isKeyPressed(binding.controls.jump))
		{
			dispatchAction(binding, PlayerAction::Jump);
		}

		if (sf::Keyboard::isKeyPressed(binding.controls.crouch))
		{
			dispatchAction(binding, PlayerAction::CrouchStart);
		}
		else
		{
			dispatchAction(binding, PlayerAction::CrouchEnd);
		}
	}
}

void InputSystem::dispatchAction(PlayerBinding binding, PlayerAction action) const
{
	if (binding.actionCallback)
	{
		binding.actionCallback(action);
	}
}

void InputSystem::setContext(InputContext context)
{
	currentContext = context;
}

InputContext InputSystem::getContext() const
{
	return currentContext;
}

void InputSystem::addPlayer(const PlayerControls& controls, ActionCallback callback)
{
	players.push_back(PlayerBinding{ controls,callback });
}