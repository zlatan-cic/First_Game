#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "State.h"

class Game
{
public:
    Game();

    void run();

    // State changes are not applied right away, but after the current loop step
    void pushState(std::unique_ptr<State> state);   // new state ON TOP of the current one
    void popState();                                // remove the top state
    void changeState(std::unique_ptr<State> state); // remove all, keep only the new one

    sf::RenderWindow& getWindow();

    void drawCenteredText(const std::string& message, float y, unsigned int size,
        sf::Color color = sf::Color::White);

private:
    enum class StateAction
    {
        None,
        Push,
        Pop,
        Change
    };

    void processEvents();
    void update(float dt);
    void render();
    void applyStateChange();

    sf::RenderWindow m_Window;
    sf::Font font;

    // State stack: the last one in the list is active (on top)
    std::vector<std::unique_ptr<State>> states;

    StateAction pendingAction = StateAction::None;
    std::unique_ptr<State> pendingState;
};