#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "InputSystem.h"



class Game
{
private:
	void processEvents();
	void update(float dt);
	void render();

	sf::RenderWindow m_Window;
	sf::CircleShape m_Shape;

	Player player;
	InputSystem inputSystem;

	sf::RectangleShape floor;

public:
	Game();
	void run();


};