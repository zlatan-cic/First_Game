#include "Game.h"

Game::Game()
	: m_Window(sf::VideoMode({ 800, 600 }), "SFML test game loop"),
	inputSystem(PlayerControls{
		sf::Keyboard::Key::A,
		sf::Keyboard::Key::D,
		sf::Keyboard::Key::W,
		sf::Keyboard::Key::S
		}),
	playerController(player)
{
	floor.setSize({ 800.f, 50.f });
	floor.setPosition({ 0.f, 550.f });
	floor.setFillColor(sf::Color::Green);

	inputSystem.setActionCallback(
		[this](PlayerAction action)
		{
			playerController.handleAction(action);
		}
	);
}

void Game::run()
{
	sf::Clock clock;

	// This is a core of game loop!!!!
	while (m_Window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		processEvents();
		update(dt);
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

void Game::update(float dt)
{
	player.resetInput();
	inputSystem.update();
	player.update(dt);
}

void Game::render()
{
	m_Window.clear();
	m_Window.draw(floor);
	
	player.render(m_Window);
	m_Window.display();
}