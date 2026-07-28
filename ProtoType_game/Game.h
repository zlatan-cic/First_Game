#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Game
{
private:
	void processEvents();
	void update(float dt);
	void render();

	sf::RenderWindow m_Window;
	sf::CircleShape m_Shape;

	Player player;
	sf::RectangleShape floor;

public:
	Game();
	void run();


};