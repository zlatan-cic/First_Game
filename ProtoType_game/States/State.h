#pragma once
#include <SFML/Graphics.hpp>

class Game;

// Contract for all states: every state MUST have these 4 functions
class State
{
public:
    explicit State(Game& game) : game(game) {}
    virtual ~State() = default;

    virtual void init() = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

protected:
    Game& game;
};