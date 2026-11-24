#pragma once
#include "State.h"

class PauseState : public State
{
public:
    explicit PauseState(Game& game);

    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};