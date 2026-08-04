#include "Player.h"

Player::Player()
	    :speed(250.f),
		gravity(900.f),
		jumpForce(-500.f),
		movement(0.f, 0.f),
		velocity(0.f, 0.f),
		standardHeight(50.f),
		crouchingHeight(25.f),
		currentState(PlayerState::Falling)
{
	shape.setSize({ 50.f,50.f });
	shape.setFillColor(sf::Color::White); /// later...
	shape.setPosition({ 150.f,450.f });
}

bool Player::isGrounded() const
{
	return  currentState == PlayerState::Idle ||
			currentState == PlayerState::Moving ||
			currentState == PlayerState::Crouching;
}

bool Player::isCrouching() const
{
	return currentState == PlayerState::Crouching;
}

void Player::handleAction(PlayerAction action)
{
	switch (action)
	{
	case PlayerAction::MoveLeft:
		movement.x -= speed;
		break;

	case PlayerAction::MoveRight:
		movement.x += speed;
		break;

	case PlayerAction::Jump:
		if (isGrounded() && !isCrouching())
		{
			velocity.y = jumpForce;
			currentState = PlayerState::Jumping;
		}
		break;

	case PlayerAction::CrouchStart:
		if (isGrounded() && !isCrouching())
		{
			const float bottom =
				shape.getPosition().y + shape.getSize().y;
			shape.setSize({ shape.getSize().x, crouchingHeight });

			shape.setPosition({ shape.getPosition().x, bottom - crouchingHeight });

			currentState = PlayerState::Crouching;
		}
		break;

	case PlayerAction::CrouchEnd:
		if (isCrouching())
		{
			const float bottom =
				shape.getPosition().y + shape.getSize().y;
			shape.setSize({
				shape.getSize().x , 
				standardHeight
			});
			
			shape.setPosition({
				shape.getPosition().x,
				bottom - standardHeight
				});


			currentState = PlayerState::Idle;
		}
		break;
	}
}

void Player::resetInput()
{
	movement.x = 0.f;
}

void Player::update(float dt)
{
	// Gravity
	velocity.y += gravity * dt;

	if (velocity.y > 0.f &&
		currentState == PlayerState::Jumping)
	{
		currentState = PlayerState::Falling;
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
			currentState = PlayerState::Idle;
		}
	}

	
}



void Player::render(sf::RenderWindow& m_Window)
{
	m_Window.draw(shape);
}
