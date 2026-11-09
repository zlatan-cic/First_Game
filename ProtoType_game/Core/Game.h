#pragma once

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "InputSystem.h"
#include "PlayerController.h"
#include <vector>
#include "CollisionSystem.h"
#include "GameState.h"
#include <string>

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
    void handleFallOut(Player& player);
    void handleScreenEdges(Player& player);

    void checkGameOver();

    void drawCenteredText(const std::string& message, float y, unsigned int size);

    sf::RenderWindow m_Window;
    InputSystem inputSystem;

    CollisionSystem collisionSystem;

    std::vector<PlayerConfig> playerConfigs;


   

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

    GameState currentState;

    std::string winnerName;

    sf::Font font;
    



public:
    Game();



    void run();
};