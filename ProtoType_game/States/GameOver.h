#pragma once
#include "State.h"
#include <string>

class GameOverState : public State
{
public:
    GameOverState(Game& game, const std::string& winnerName);

    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    std::string winnerName;
};