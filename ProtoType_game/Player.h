#pragma once
#include <SFML/Graphics.hpp>

class Player
{
private:
	sf::RectangleShape shape;

	float speed;
	float gravity;
	float jumpForce;

	sf::Vector2f velocity;

	bool isGrounded;

public:
	Player();

	void update(float dt);
	void render(sf::RenderWindow& m_Window);

};