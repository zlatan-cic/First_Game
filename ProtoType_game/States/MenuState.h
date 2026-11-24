#pragma once
#include "State.h"
#include "LevelConfig.h"
#include <string>
#include <vector>

class MenuState : public State
{
public:
    explicit MenuState(Game& game);

    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    struct MenuOption
    {
        std::string label;
        bool enabled = true;
    };

    void handleKey(sf::Keyboard::Key key);
    void confirmSelection();
    void changeLevel(int direction);

    std::vector<MenuOption> options;
    int selectedIndex = 0;
    std::string message;

    std::vector<LevelConfig> levels;
};