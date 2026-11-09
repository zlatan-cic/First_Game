#include "Game.h"
#include "ConfigLoader.h"
#include <iostream>

Game::Game()
    : m_Window(
        sf::VideoMode({ 800, 600 }),
        "SFML test game loop"
    ),

    // Prvo procitaj igrace iz JSON-a (uvek vraca tacno 2)
    playerConfigs(loadPlayerConfigs("players.json", 2)),

    // PLAYER 1
    player1(playerConfigs[0]),
    playerController1(player1),

    // PLAYER 2
    player2(playerConfigs[1]),
    playerController2(player2),

    // GameState
    currentState(GameState::Playing)

{
    initUI();
    initPlatforms();
    initInput();

    

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
                if (currentState == GameState::Playing)
                {
                    currentState = GameState::Paused;
                    inputSystem.setContext(InputContext::Pause);
                }
                else if (currentState == GameState::Paused)
                {
                    currentState = GameState::Playing;
                    inputSystem.setContext(InputContext::Gameplay);
                }
            }

            if (keyPressed->code == sf::Keyboard::Key::F)
            {
                if (currentState == GameState::Playing)
                {
                    const auto player1Bounds = player1.getBounds();
                    const auto player2Bounds = player2.getBounds();

                    const float attackRange = 40.f;

                    const float player1Right =
                        player1Bounds.position.x + player1Bounds.size.x;

                    const float distance =
                        player2Bounds.position.x - player1Right;
                    std::cout << "ATTACK!!!!!\n";

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
}

void Game::update(float dt)
{
    if (currentState != GameState::Playing)
    {
        return;
    }

    // Reset input from previous frame
    player1.resetInput();
    player2.resetInput();

    // Read keyboard
    inputSystem.update();

    // Update players
    player1.update(dt);
    player2.update(dt);

    // Platform collision 
    handlePlatformsCollisions(player1);
    handlePlatformsCollisions(player2);

    // Screen Edges
    handleScreenEdges(player1);
    handleScreenEdges(player2);

    player1HealthBar.setSize({
        200.f * (player1.getHealth() / 100.f),
        20.f
        });

    player2HealthBar.setSize({
        200.f * (player2.getHealth() / 100.f),
        20.f
        });
}

void Game::render()
{
    m_Window.clear();


    for (const auto& platform : platforms)
    {
        m_Window.draw(platform);
    }

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

void Game::initUI()
{
    player1HealthBar.setSize({ 200.f, 20.f });
    player1HealthBar.setPosition({ 20.f, 20.f });
    player1HealthBar.setFillColor(sf::Color::Green);

    player2HealthBar.setSize({ 200.f, 20.f });
    player2HealthBar.setPosition({ 580.f, 20.f });
    player2HealthBar.setFillColor(sf::Color::Red);
}

void Game::initPlatforms()
{
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
}

void Game::initInput()
{
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

void Game::handlePlatformsCollisions(Player& player)
{
    for (const auto& platform : platforms)
    {
        const CollisionInfo info = collisionSystem.check(player.getBounds(), platform.getGlobalBounds());

        if (info.side == CollisionSide::Top && player.isMovingDown())
        {
            player.landOn(platform.getPosition().y);
        }
    }
}

void Game::handleFallOut(Player& player)
{
    player.loseLife();
    std::cout << player.getName() << " lives: " << player.getLives() << '\n';

    if (player.getLives() > 0)
    {
        player.resetPosition();
    }
    else
    {
        std::cout << player.getName() << " Game Over!\n";
        player.setAlive(false);
    }
}

void Game::handleScreenEdges(Player& player)
{
    const sf::FloatRect screen({ 0.f, 0.f }, sf::Vector2f(m_Window.getSize()));
    const ScreenEdgeInfo edges = collisionSystem.checkScreenEdges(player.getBounds(), screen);

    if (edges.outLeft)
    {
        player.setPositionX(screen.position.x);
    }

    if (edges.outRight)
    {
        player.setPositionX(screen.size.x - player.getBounds().size.x);
    }

    if (edges.outBottom)
    {
        handleFallOut(player);
    }
}