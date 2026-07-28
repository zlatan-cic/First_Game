#include "Player.h"

Player::Player() 
		:speed(250.f),
		 gravity(900.f),
		 jumpForce(-500.f),
		 velocity(0.f, 0.f),
		 isGrounded(false)
{
	shape.setSize({ 50.f,50.f });
	shape.setFillColor(sf::Color::White); /// later...
	shape.setPosition({ 150.f,450.f });
}

void Player::update(float dt)
{
	sf::Vector2 movement(0.0f, 0.0f);

	velocity.x = 0.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		movement.x -= speed * dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		movement.x += speed * dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && isGrounded)
	{
		velocity.y = jumpForce;
		isGrounded = false;
	}

	velocity.y += gravity * dt;
	shape.move(velocity * dt);

	const float floorY = 550.f;
	if (shape.getPosition().y + shape.getSize().y >= floorY)
	{
		shape.setPosition({
			shape.getPosition().x,
			floorY - shape.getSize().y
		});

		velocity.y = 0.f;
		isGrounded = true;
	}

	shape.move(movement);
}

void Player::render(sf::RenderWindow& m_Window)
{
	m_Window.draw(shape);
}
