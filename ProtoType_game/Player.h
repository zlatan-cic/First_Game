#pragma once
#include <SFML/Graphics.hpp>

enum class PlayerState
{
	Idle,
	Moving,
	Crouching,
	Jumping,
	Falling

};

class Player
{
private:
	sf::RectangleShape shape;

	float speed;
	float gravity;
	float jumpForce;

	sf::Vector2f velocity;

	float standardHeight;
	float crouchingHeight;

	PlayerState currentState;

public:
	Player();

	void update(float dt);
	void render(sf::RenderWindow& m_Window);

};