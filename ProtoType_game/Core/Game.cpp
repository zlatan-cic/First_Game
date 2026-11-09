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
    currentState(GameState::Menu)

{
    initUI();
    initPlatforms();
    initInput();
    initMenu();
   
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
            switch (currentState)
            {
            case GameState::Menu:
                handleMenuKey(keyPressed->code);
                break;

            case GameState::Playing:
            case GameState::Paused:
                handleGameKey(keyPressed->code);
                break;

            case GameState::GameOver:
                handleGameOverKey(keyPressed->code);
                break;
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

    checkGameOver();
}

void Game::render()
{
    m_Window.clear();

    if (currentState == GameState::Menu)
    {
        drawMenu();
        m_Window.display();
        return;
    }

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

    if (currentState == GameState::Paused)
    {
        drawCenteredText("PAUSED", 280.f, 48);
        drawCenteredText("Esc - continue    Q - menu", 340.f, 18, sf::Color(180, 178, 169));
    }
    else if (currentState == GameState::GameOver)
    {
        drawCenteredText(winnerName + " wins!", 280.f, 40);
        drawCenteredText("Press Enter for menu", 340.f, 18, sf::Color(180, 178, 169));
    }


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

    
    if (!font.openFromFile("Assets/Fonts/Tiny5Duo-Medium.ttf"))
    {
        std::cout << "Problem with Font\n";
    }
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

void Game::checkGameOver()
{
    if (player1.isAlive() && player2.isAlive())
    {
        return;
    }
    currentState = GameState::GameOver;

    inputSystem.setContext(InputContext::Pause);

    if (player1.isAlive())
    {
        winnerName = player1.getName();
        std::cout << player2.getName() << " defeated!\n";
    }
    else if (player2.isAlive())
    {
        winnerName = player2.getName();
        std::cout << player1.getName() << " defeated!\n";
    }
    else
    {
        winnerName = "Nobody";
        std::cout << "Both players defeated!\n";
    }

    std::cout << "Winner: " << winnerName << '\n';
}

//void Game::drawCenteredText(const std::string& message, float y, unsigned int size) 
//{
//    sf::Text text(font, message, size);
//
//    const auto bounds = text.getLocalBounds();
//    text.setOrigin(bounds.position + bounds.size / 2.f);
//
//    text.setPosition({ m_Window.getSize().x / 2.f, y });
//
//    m_Window.draw(text);
//}

void Game::initMenu()
{
    menuOptions = {
        { "Player vs Player", true },
        { "Player vs AI", false },
        { "Online", false },
        { "Exit", true }
    };

    selectedIndex = 0;
}

void Game::drawCenteredText(const std::string& message, float y, unsigned int size, sf::Color color)
{
    sf::Text text(font, message, size);
    text.setFillColor(color);

    const auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.position + bounds.size / 2.f);

    text.setPosition({ m_Window.getSize().x / 2.f, y });

    m_Window.draw(text);
}

void Game::drawMenu()
{
    const sf::Color normal(241, 239, 232);
    const sf::Color selected(250, 199, 117);
    const sf::Color disabled(136, 135, 128);
    const sf::Color hint(180, 178, 169);

    drawCenteredText("First Game", 120.f, 56, normal);
    drawCenteredText("Welcome! Choose game mode", 185.f, 20, hint);

    const float startY = 260.f;
    const float spacing = 50.f;

    for (int i = 0; i < static_cast<int>(menuOptions.size()); ++i)
    {
        const MenuOption& option = menuOptions[i];

        std::string label = option.label;
        if (!option.enabled)
        {
            label += " (soon...)";
        }

        sf::Color color = option.enabled ? normal : disabled;

        if (i == selectedIndex)
        {
            label = "> " + label + " <";
            color = selected;
        }

        drawCenteredText(label, startY + i * spacing, 26, color);
    }

    if (!menuMessage.empty())
    {
        drawCenteredText(menuMessage, 480.f, 20, selected);
    }

    drawCenteredText("Up/Down - choose    Enter - confirm", 550.f, 16, hint);
}

void Game::handleMenuKey(sf::Keyboard::Key key)
{
    const int optionCount = static_cast<int>(menuOptions.size());

    if (key == sf::Keyboard::Key::Up || key == sf::Keyboard::Key::W)
    {
        selectedIndex = (selectedIndex - 1 + optionCount) % optionCount;
        menuMessage.clear();
    }
    else if (key == sf::Keyboard::Key::Down || key == sf::Keyboard::Key::S)
    {
        selectedIndex = (selectedIndex + 1) % optionCount;
        menuMessage.clear();
    }
    else if (key == sf::Keyboard::Key::Enter)
    {
        const MenuOption& option = menuOptions[selectedIndex];

        if (!option.enabled)
        {
            menuMessage = "Coming soon!";
        }
        else if (option.label == "Player vs Player")
        {
            startNewGame();
        }
        else if (option.label == "Exit")
        {
            m_Window.close();
        }
    }
    else if (key == sf::Keyboard::Key::Escape)
    {
        m_Window.close();
    }
}

void Game::handleGameKey(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Key::Escape)
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
    else if (key == sf::Keyboard::Key::Q && currentState == GameState::Paused)
    {
        currentState = GameState::Menu;
    }
    else if (key == sf::Keyboard::Key::F && currentState == GameState::Playing)
    {
        handleAttack();
    }
}


void Game::handleGameOverKey(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Key::Enter)
    {
        currentState = GameState::Menu;
    }
}

void Game::handleAttack()
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

void Game::startNewGame()
{
    player1 = Player(playerConfigs[0]);
    player2 = Player(playerConfigs[1]);

    winnerName.clear();
    menuMessage.clear();

    inputSystem.setContext(InputContext::Gameplay);
    currentState = GameState::Playing;
}