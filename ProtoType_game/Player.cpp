#include "Player.h"

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

void Player::handleAction(PlayerAction action)
{
	switch (action)
	{
	case PlayerAction::MoveLeft:
		if (!isCrouching())
		{
			movement.x -= speed;
			horizontalState = HorizontalPlayerState::MovingLeft;
		}
		break;

	case PlayerAction::MoveRight:
		if (!isCrouching())
		{
			movement.x += speed;
			horizontalState = HorizontalPlayerState::MovingRight;
		}
		break;

	case PlayerAction::Jump:
		
		if (isGrounded() && !isCrouching())
		{
			velocity.y = jumpForce;
			verticalState = VerticalPlayerState::Jumping;
		}
		break;

	case PlayerAction::CrouchStart:
		if (isGrounded() && !isCrouching())
		{
			const float bottom =
				shape.getPosition().y + shape.getSize().y;
			shape.setSize({ shape.getSize().x, crouchingHeight });

			shape.setPosition({ shape.getPosition().x, bottom - crouchingHeight });

			verticalState = VerticalPlayerState::Crouching;
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


			verticalState = VerticalPlayerState::Standing;
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

	if (velocity.y > 0.f &&
		verticalState == VerticalPlayerState::Jumping)
	{
		verticalState = VerticalPlayerState::Falling;
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
			verticalState = VerticalPlayerState::Standing;
		}
	}

	
}



void Player::render(sf::RenderWindow& m_Window)
{
	m_Window.draw(shape);
}
