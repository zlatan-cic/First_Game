#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerAction.h"

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
	sf::Vector2f movement;

	sf::Vector2f velocity;

	float standardHeight;
	float crouchingHeight;

	PlayerState currentState;

	bool isGrounded() const;
	bool isCrouching() const;

public:
	Player();

	void resetInput();

	void handleAction(PlayerAction action);

	void update(float dt);
	void render(sf::RenderWindow& m_Window);


};