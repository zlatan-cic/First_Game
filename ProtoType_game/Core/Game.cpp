#include "Game.h"
#include "MenuState.h"
#include <iostream>

Game::Game()
    : m_Window(
        sf::VideoMode({ 800, 600 }),
        "SFML test game loop"
    )
{
    if (!font.openFromFile("Assets/Fonts/Tiny5Duo-Medium.ttf"))
    {
        std::cout << "Problem with Font\n";
    }

    // The game starts in the menu
    changeState(std::make_unique<MenuState>(*this));
    applyStateChange();
}

void Game::run()
{
    sf::Clock clock;

    while (m_Window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        processEvents();
        applyStateChange();

        update(dt);
        applyStateChange();

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
            return;
        }

        // Only the top state gets the input
        if (!states.empty())
        {
            states.back()->handleEvent(*event);
        }
    }
}

void Game::update(float dt)
{
    // Only the top state is updated (that's why the game stops during pause)
    if (!states.empty())
    {
        states.back()->update(dt);
    }
}

void Game::render()
{
    m_Window.clear();

    // ALL states are drawn, bottom to top (that's why the game is visible behind pause)
    for (const auto& state : states)
    {
        state->draw(m_Window);
    }

    m_Window.display();
}

void Game::pushState(std::unique_ptr<State> state)
{
    pendingAction = StateAction::Push;
    pendingState = std::move(state);
}

void Game::popState()
{
    pendingAction = StateAction::Pop;
}

void Game::changeState(std::unique_ptr<State> state)
{
    pendingAction = StateAction::Change;
    pendingState = std::move(state);
}

void Game::applyStateChange()
{
    switch (pendingAction)
    {
    case StateAction::None:
        return;

    case StateAction::Push:
        states.push_back(std::move(pendingState));
        states.back()->init();
        break;

    case StateAction::Pop:
        if (!states.empty())
        {
            states.pop_back();
        }
        break;

    case StateAction::Change:
        states.clear();
        states.push_back(std::move(pendingState));
        states.back()->init();
        break;
    }

    pendingAction = StateAction::None;
}

sf::RenderWindow& Game::getWindow()
{
    return m_Window;
}

GameSettings& Game::getSettings()
{
    return settings;
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