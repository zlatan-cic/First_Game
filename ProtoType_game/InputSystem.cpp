#include "InputSystem.h"

#include <utility>

InputSystem::InputSystem(const PlayerControls& controls)
	: controls(controls),
	currentContext(InputContext::Gameplay)
{

}

void InputSystem::setActionCallback(ActionCallback callback)
{
	actionCallback = std::move(callback);
}

void InputSystem::update()
{
	if (currentContext != InputContext::Gameplay)
	{
		return;
	}
	if (sf::Keyboard::isKeyPressed(controls.moveLeft))
	{
		dispatchAction(PlayerAction::MoveLeft);
	}

	if (sf::Keyboard::isKeyPressed(controls.moveRight))
	{
		dispatchAction(PlayerAction::MoveRight);
	}

	if (sf::Keyboard::isKeyPressed(controls.jump))
	{
		dispatchAction(PlayerAction::Jump);
	}

	if (sf::Keyboard::isKeyPressed(controls.crouch))
	{
		dispatchAction(PlayerAction::CrouchStart);
	}
	else
	{
		dispatchAction(PlayerAction::CrouchEnd);
	}
}

void InputSystem::dispatchAction(PlayerAction action)
{
	if (actionCallback)
	{
		actionCallback(action);
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