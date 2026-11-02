#pragma once

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "InputSystem.h"
#include "PlayerController.h"
#include <vector>
#include "CollisionSystem.h"

class Game
{
private:
    void processEvents();
    void update(float dt);
    void render();

    void initUI();
    void initPlatforms();
    void initInput();

    void handlePlatformsCollisions(Player& player);

    sf::RenderWindow m_Window;
    InputSystem inputSystem;

    CollisionSystem collisionSystem;

    // Player 1
    Player player1;
    PlayerController playerController1;

    // Player 2
    Player player2;
    PlayerController playerController2;

    std::vector<sf::RectangleShape> platforms;

    // HP Bar
    sf::RectangleShape player1HealthBar;
    sf::RectangleShape player2HealthBar;



public:
    Game();

    void run();
};