#include "MenuState.h"
#include "Game.h"
#include "PlayState.h"
#include "ConfigLoader.h"

MenuState::MenuState(Game& game) : State(game)
{}

void MenuState::init()
{
    options = {
        { "Player vs Player", true },
        { "Player vs AI", false },
        { "Online", false },
        { "Exit", true }
    };

    selectedIndex = 0;
    message.clear();

    levels = loadLevelConfigs("levels.json");

    // The remembered level may not exist anymore (e.g. levels.json changed)
    if (game.getSettings().levelIndex >= levels.size())
    {
        game.getSettings().levelIndex = 0;
    }
}

void MenuState::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        handleKey(keyPressed->code);
    }
}

void MenuState::handleKey(sf::Keyboard::Key key)
{
    const int optionCount = static_cast<int>(options.size());

    if (key == sf::Keyboard::Key::Up || key == sf::Keyboard::Key::W)
    {
        // From the top, wrap to the bottom
        selectedIndex = (selectedIndex - 1 + optionCount) % optionCount;
        message.clear();
    }
    else if (key == sf::Keyboard::Key::Down || key == sf::Keyboard::Key::S)
    {
        // From the bottom, wrap to the top
        selectedIndex = (selectedIndex + 1) % optionCount;
        message.clear();
    }
    else if (key == sf::Keyboard::Key::Left || key == sf::Keyboard::Key::A)
    {
        changeLevel(-1);
    }
    else if (key == sf::Keyboard::Key::Right || key == sf::Keyboard::Key::D)
    {
        changeLevel(1);
    }
    else if (key == sf::Keyboard::Key::Enter)
    {
        confirmSelection();
    }
    else if (key == sf::Keyboard::Key::Escape)
    {
        game.getWindow().close();
    }
}

void MenuState::confirmSelection()
{
    const MenuOption& option = options[selectedIndex];

    if (!option.enabled)
    {
        message = "Coming soon!";
    }
    else if (option.label == "Player vs Player")
    {
        // New game = new PlayState, everything is created fresh in its init()
        const LevelConfig& level = levels[game.getSettings().levelIndex];
        game.changeState(std::make_unique<PlayState>(game, level));
    }
    else if (option.label == "Exit")
    {
        game.getWindow().close();
    }
}

void MenuState::changeLevel(int direction)
{
    const int levelCount = static_cast<int>(levels.size());
    const int current = static_cast<int>(game.getSettings().levelIndex);

    // Same wrap-around trick as the menu options
    const int next = (current + direction + levelCount) % levelCount;

    game.getSettings().levelIndex = static_cast<std::size_t>(next);
    message.clear();
}

void MenuState::update(float)
{
    // The menu has no physics, nothing to update
}

void MenuState::draw(sf::RenderWindow&)
{
    const sf::Color normal(241, 239, 232);
    const sf::Color selected(250, 199, 117);
    const sf::Color disabled(136, 135, 128);
    const sf::Color hint(180, 178, 169);

    game.drawCenteredText("First Game", 120.f, 56, normal);
    game.drawCenteredText("Welcome! Choose game mode", 175.f, 20, hint);

    const LevelConfig& level = levels[game.getSettings().levelIndex];
    game.drawCenteredText("Level: < " + level.name + " >", 215.f, 22, normal);

    const float startY = 260.f;
    const float spacing = 50.f;

    for (int i = 0; i < static_cast<int>(options.size()); ++i)
    {
        const MenuOption& option = options[i];

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

        game.drawCenteredText(label, startY + i * spacing, 26, color);
    }

    if (!message.empty())
    {
        game.drawCenteredText(message, 480.f, 20, selected);
    }

    game.drawCenteredText("Left/Right - level", 525.f, 16, hint);
    game.drawCenteredText("Up/Down - choose    Enter - confirm", 555.f, 16, hint);
}