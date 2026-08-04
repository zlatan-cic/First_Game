#include "Player.h"

Player::Player() 
		:speed(250.f),
		 gravity(900.f),
		 jumpForce(-500.f),
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

void Player::update(float dt)
{
	sf::Vector2 movement(0.0f, 0.0f);

	velocity.x = 0.f;

	// Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		movement.x -= speed * dt;
	}

	// Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		movement.x += speed * dt;
	}

	// Chrouch
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && isGrounded() && !isCrouching())
	{
		const float bottom = shape.getPosition().y + shape.getSize().y;
		shape.setSize({ shape.getSize().x, crouchingHeight });
		shape.setPosition({ shape.getPosition().x, bottom - crouchingHeight});


		currentState = PlayerState::Crouching;
	}
	else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && isCrouching())
	{
		const float bottom = shape.getPosition().y + shape.getSize().y;

		shape.setSize({ shape.getSize().x, standardHeight });
		shape.setPosition({ shape.getPosition().x, bottom - standardHeight });

		currentState = PlayerState::Idle;
	}

	// Jump
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && isGrounded() && !isCrouching())
	{
		velocity.y = jumpForce;
		currentState = PlayerState::Jumping;
	}

	/// Gravity
	velocity.y += gravity * dt;
	if (velocity.y > 0.f && currentState == PlayerState::Jumping)
	{
		currentState = PlayerState::Falling;
	}
	shape.move({ 0.f, velocity.y * dt });

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

	shape.move(movement);
}



void Player::render(sf::RenderWindow& m_Window)
{
	m_Window.draw(shape);
}
