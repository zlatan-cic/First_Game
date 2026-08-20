#include "Player.h"
#include <iostream>

Player::Player()
	    :speed(250.f),
		gravity(900.f),
		jumpForce(-500.f),
		movement(0.f, 0.f),
		velocity(0.f, 0.f),
		standardHeight(50.f),
		crouchingHeight(25.f),
		horizontalState(HorizontalPlayerState::Standing),
		verticalState(VerticalPlayerState::Falling)
{
	shape.setSize({ 50.f,50.f });
	shape.setFillColor(sf::Color::White); /// later...
	shape.setPosition({ 150.f,450.f });
}

bool Player::isGrounded() const
{
	return verticalState == VerticalPlayerState::Crouching || verticalState == VerticalPlayerState::Standing;
}

bool Player::isCrouching() const
{
	return verticalState == VerticalPlayerState::Crouching;
}

bool Player::tryVerticalStateTransition(VerticalPlayerState nextState)
{
	if (verticalState == VerticalPlayerState::Standing && nextState == VerticalPlayerState::Jumping)
	{
		verticalState = nextState;
		return true;
	}
	else if(verticalState == VerticalPlayerState::Falling && nextState == VerticalPlayerState::Standing)
	{
		verticalState = nextState;
		return true;
	}
	else if(verticalState == VerticalPlayerState::Jumping && nextState == VerticalPlayerState::Falling)
	{
		verticalState = nextState;
		return true;
	}
	else if(verticalState == VerticalPlayerState::Standing && nextState == VerticalPlayerState::Crouching)
	{
		verticalState = nextState;
		return true;
	}
	else if(verticalState == VerticalPlayerState::Crouching && nextState == VerticalPlayerState::Standing)
	{
		verticalState = nextState;
		return true;
	}
	else
	{
		return false;
	}
	
}


void Player::handleAction(PlayerAction action)
{
	switch (action)
	{
	case PlayerAction::MoveLeft:
		if (!isCrouching())
		{
			movement.x -= speed;
			horizontalState = HorizontalPlayerState::MovingLeft;
			std::cout << "MoveLeft!!!\n";
		}
		break;

	case PlayerAction::MoveRight:
		if (!isCrouching())
		{
			movement.x += speed;
			horizontalState = HorizontalPlayerState::MovingRight;
			std::cout << "MoveRight!!!\n";
		}
		break;

	case PlayerAction::Jump:
			if (tryVerticalStateTransition(VerticalPlayerState::Jumping))
			{
				velocity.y = jumpForce;
				std::cout << "State Jump!!!\n";
			}
		break;

	case PlayerAction::CrouchStart:

		if (tryVerticalStateTransition(VerticalPlayerState::Crouching))
		{
			const float bottom = shape.getPosition().y + shape.getSize().y;

			shape.setSize({ shape.getSize().x, crouchingHeight });
			shape.setPosition({ shape.getPosition().x, bottom - crouchingHeight });
			std::cout << "CrouchStart!!!\n";
		}
		break; 
	case PlayerAction::CrouchEnd:
		/*if (tryVerticalStateTransition(VerticalPlayerState::Standing))
		{
			const float bottom = shape.getPosition().y + shape.getSize().y;
			shape.setSize({ shape.getSize().x, standardHeight });
			shape.setPosition({ shape.getPosition().x, bottom - standardHeight });
			std::cout << "CrouchEnd!!!\n";
		}*/
		if (isCrouching())
		{
			if (tryVerticalStateTransition(VerticalPlayerState::Standing))
			{
				const float bottom = shape.getPosition().y + shape.getSize().y;

				shape.setSize({ shape.getSize().x, standardHeight });
				shape.setPosition({ shape.getPosition().x, bottom - standardHeight });
				std::cout << "CrouchEnd!!!\n";
			}
		}
		break;
	}
}

void Player::resetInput()
{
	movement.x = 0.f;
	horizontalState = HorizontalPlayerState::Standing;
}

void Player::update(float dt)
{
	// Gravity
	velocity.y += gravity * dt;

	if (velocity.y > 0.f && verticalState == VerticalPlayerState::Jumping)
	{
		tryVerticalStateTransition(VerticalPlayerState::Falling);
	}

	// horizontal movement
	shape.move({ movement.x * dt, 0.f });

	// vertical movement
	shape.move({ 0.f, velocity.y * dt });

	// temporary ground collision
	const float floorY = 550.f;

	if (shape.getPosition().y + shape.getSize().y >= floorY)
	{
		shape.setPosition({
			shape.getPosition().x,
			floorY - shape.getSize().y
		});

		velocity.y = 0.f;

		if (!isCrouching())
		{
			tryVerticalStateTransition(VerticalPlayerState::Standing);
		}
	}

	
}



void Player::render(sf::RenderWindow& m_Window)
{
	m_Window.draw(shape);
}
