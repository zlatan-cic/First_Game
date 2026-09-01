#include "Game.h"

Game::Game()
    : m_Window(
        sf::VideoMode({ 800, 600 }),
        "SFML test game loop"
    ),

    // PLAYER 1
    player1({ 150.f, 450.f }),

    inputSystem1(PlayerControls{
        sf::Keyboard::Key::A,
        sf::Keyboard::Key::D,
        sf::Keyboard::Key::W,
        sf::Keyboard::Key::S
        }),

    playerController1(player1),

    // PLAYER 2
    player2({ 600.f, 450.f }),

    inputSystem2(PlayerControls{
        sf::Keyboard::Key::Left,
        sf::Keyboard::Key::Right,
        sf::Keyboard::Key::Up,
        sf::Keyboard::Key::Down
        }),

    playerController2(player2)
{
    // Floor
    floor.setSize({ 800.f, 50.f });
    floor.setPosition({ 0.f, 550.f });
    floor.setFillColor(sf::Color::Green);

    // InputSystem 1 -> PlayerController 1
    inputSystem1.setActionCallback(
        [this](PlayerAction action)
        {
            playerController1.handleAction(action);
        }
    );

    // InputSystem 2 -> PlayerController 2
    inputSystem2.setActionCallback(
        [this](PlayerAction action)
        {
            playerController2.handleAction(action);
        }
    );
}

void Game::run()
{
    sf::Clock clock;

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
    while (const std::optional event = m_Window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_Window.close();
        }

        if (const auto* keyPressed =
            event->getIf<sf::Event::KeyPressed>())
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
    // Reset input from previous frame
    player1.resetInput();
    player2.resetInput();

    // Read keyboard
    inputSystem1.update();
    inputSystem2.update();

    // Update players
    player1.update(dt);
    player2.update(dt);
}

void Game::render()
{
    m_Window.clear();

    m_Window.draw(floor);

    player1.render(m_Window);
    player2.render(m_Window);

    m_Window.display();
}