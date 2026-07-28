#include "Game.h"

Game::Game() :m_Window(sf::VideoMode({ 800, 600 }), "SFML test game loop")
{
	
	m_Shape.setRadius(50.f);
	m_Shape.setFillColor(sf::Color::Blue);
	m_Shape.setPosition({ 350.f, 250.f });
}

void Game::run()
{
	// This is a core of game loop!!!!
	while (m_Window.isOpen())
	{
		processEvents();
		update();
		render();
	}
}

void Game::processEvents()
{
	// All process events are here!
	while (const std::optional event = m_Window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_Window.close();
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->code == sf::Keyboard::Key::Escape)
			{
				m_Window.close();
			}
		}
	}
}

void Game::update()
{
	// Game logic here
}

void Game::render()
{
	m_Window.clear();
	m_Window.draw(m_Shape);
	m_Window.display();
}