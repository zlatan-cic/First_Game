#pragma once

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "InputSystem.h"
#include "PlayerController.h"
#include <vector>

class Game
{
private:
    void processEvents();
    void update(float dt);
    void render();

    sf::RenderWindow m_Window;

    // Player 1
    Player player1;
    InputSystem inputSystem1;
    PlayerController playerController1;

    // Player 2
    Player player2;
    InputSystem inputSystem2;
    PlayerController playerController2;

    //sf::RectangleShape floor;
    std::vector<sf::RectangleShape> platforms; // test

public:
    Game();

    void run();
};