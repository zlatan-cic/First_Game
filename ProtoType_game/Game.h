#pragma once
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	void run();

private:
	void processEvents();
	void update();
	void render();

	sf::RenderWindow m_Window;
	sf::CircleShape m_Shape;
};