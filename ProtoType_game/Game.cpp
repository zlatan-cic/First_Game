#include "Game.h"
#include <iostream>

Game::Game()
    : m_Window(
        sf::VideoMode({ 800, 600 }),
        "SFML test game loop"
    ),

    // PLAYER 1
    player1({ 150.f, 450.f }, sf::Color::White),
    playerController1(player1),

    // PLAYER 2
    player2({ 600.f, 450.f }, sf::Color::Red),
    playerController2(player2)

    {
    player1HealthBar.setSize({ 200.f, 20.f });
    player1HealthBar.setPosition({ 20.f, 20.f });
    player1HealthBar.setFillColor(sf::Color::Green);

    player2HealthBar.setSize({ 200.f, 20.f });
    player2HealthBar.setPosition({ 580.f, 20.f });
    player2HealthBar.setFillColor(sf::Color::Red);

    sf::RectangleShape floor({ 600.f, 40.f });
    floor.setPosition({ 100.f, 500.f });
    floor.setFillColor(sf::Color::Green);
    platforms.push_back(floor);

    sf::RectangleShape platform1({ 250.f, 25.f });
    platform1.setPosition({ 100.f, 380.f });
    platform1.setFillColor(sf::Color::Green);
    platforms.push_back(platform1);

    sf::RectangleShape platform2({ 250.f, 25.f });
    platform2.setPosition({ 450.f, 260.f });
    platform2.setFillColor(sf::Color::Green);
    platforms.push_back(platform2);

    // Player 1 : WASD
    inputSystem.addPlayer(
        PlayerControls{
            sf::Keyboard::Key::A,
            sf::Keyboard::Key::D,
            sf::Keyboard::Key::W,
            sf::Keyboard::Key::S
        },
        [this](PlayerAction action)
        {
            playerController1.handleAction(action);
        }
    );

    inputSystem.addPlayer(
        PlayerControls{
            sf::Keyboard::Key::Left,
            sf::Keyboard::Key::Right,
            sf::Keyboard::Key::Up,
            sf::Keyboard::Key::Down
        },
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
            // TEST //  //  //  //      //  //  //  //  //  /
            // TEST //  //  //  //  //  //  //
            if (keyPressed->code == sf::Keyboard::Key::F)
            {
                const auto player1Bounds = player1.getBounds();
                const auto player2Bounds = player2.getBounds();

                const float attackRange = 40.f;

                const float player1Right =
                    player1Bounds.position.x + player1Bounds.size.x;

                const float distance =
                    player2Bounds.position.x - player1Right;

                if (distance >= 0.f && distance <= attackRange)
                {
                    player2.takeDamage(10);

                    std::cout << "Player 1 hit Player 2!\n";
                    std::cout << "Player 2 HP: "
                        << player2.getHealth()
                        << '\n';

                    if (player2.getHealth() <= 0)
                    {
                        player2.loseLife();

                        if (player2.getLives() > 0)
                        {
                            player2.resetHealth();
                            player2.resetPosition();
                        }
                        else
                        {
                            std::cout << "Player 2 Game Over!\n";
                            player2.setAlive(false);
                        }
                    }
                }
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
    inputSystem.update();

    // Update players
    player1.update(dt);
    player2.update(dt);

    // Platform collision 
    for (const auto& platform : platforms)
    {
        player1.resolvePlatformCollision(platform);
        player2.resolvePlatformCollision(platform);
    }


    if (player1.getPosition().y > 650.f)
    {
        player1.loseLife();
        std::cout << "Player 1 lives: " << player1.getLives() << '\n';

        if (player1.getLives() > 0)
        {
            player1.resetPosition();
        }
        else
        {
            std::cout << "Player 1 Game Over!\n";
            player1.setAlive(false);
        }
    }
    

    if (player2.getPosition().y > 650.f)
    {
        player2.loseLife();
        std::cout << "Player 2 lives: " << player2.getLives() << '\n';

        if (player2.getLives() > 0)
        {
            player2.resetPosition();
        }
        else
        {
            std::cout << "Player 2 Game Over!\n";
            player2.setAlive(false); /////
        }
    }

    player1HealthBar.setSize({
        200.f * (player1.getHealth() / 100.f),
        20.f
    });

    player2HealthBar.setSize({
        200.f * (player2.getHealth() / 100.f),
        20.f
    });

    //if (player1.getLives() <= 0)
    //{
    //    std::cout << "Player 1 Game Over!\n";
    //}

    //if (player2.getLives() <= 0)
    //{
    //    std::cout << "Player 2 Game Over!\n";
    //}
}

void Game::render()
{
    m_Window.clear();

    //m_Window.draw(floor);
    for (const auto& platform : platforms)
    {
        m_Window.draw(platform);
    }

    //player1.render(m_Window);
    //player2.render(m_Window);

    if (player1.isAlive())
    {
        player1.render(m_Window);
    }

    if (player2.isAlive())
    {
        player2.render(m_Window);
    }

    m_Window.draw(player1HealthBar);
    m_Window.draw(player2HealthBar);

    m_Window.display();
}