#include "GameOver.h"
#include "Game.h"
#include "MenuState.h"

GameOverState::GameOverState(Game& game, const std::string& winnerName)
    : State(game),
    winnerName(winnerName)
{}

void GameOverState::init()
{}

void GameOverState::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::Enter)
        {
            game.changeState(std::make_unique<MenuState>(game));
        }
    }
}

void GameOverState::update(float)
{}

void GameOverState::draw(sf::RenderWindow& window)
{
    sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    game.drawCenteredText(winnerName + " wins!", 280.f, 40);
    game.drawCenteredText("Press Enter for menu", 340.f, 18, sf::Color(180, 178, 169));
}