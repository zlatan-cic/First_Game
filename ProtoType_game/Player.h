#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerAction.h"

enum class HorizontalPlayerState
{
	Standing,
	MovingLeft,
	MovingRight
};

enum class VerticalPlayerState
{
	Standing,
	Jumping,
	Falling,
	Crouching
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

	HorizontalPlayerState horizontalState;
	VerticalPlayerState	verticalState;

	bool isGrounded() const;
	bool isCrouching() const;

public:
	Player();

	void resetInput();

	void handleAction(PlayerAction action);

	void update(float dt);
	void render(sf::RenderWindow& m_Window);


};