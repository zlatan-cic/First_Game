#include "PauseState.h"
#include "Game.h"
#include "MenuState.h"

PauseState::PauseState(Game& game) : State(game)
{}

void PauseState::init()
{}

void PauseState::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::Escape)
        {
            // Remove pause, the game below continues
            game.popState();
        }
        else if (keyPressed->code == sf::Keyboard::Key::Q)
        {
            game.changeState(std::make_unique<MenuState>(game));
        }
    }
}

void PauseState::update(float)
{
    // Nothing to update; the game below is not updated because it's not on top
}

void PauseState::draw(sf::RenderWindow& window)
{
    // Transparent black layer over the game
    sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    game.drawCenteredText("PAUSED", 280.f, 48);
    game.drawCenteredText("Esc - continue    Q - menu", 340.f, 18, sf::Color(180, 178, 169));
}