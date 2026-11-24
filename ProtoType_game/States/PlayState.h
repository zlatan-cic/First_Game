#pragma once
#include "State.h"
#include "Player.h"
#include "PlayerConfig.h"
#include "PlayerController.h"
#include "InputSystem.h"
#include "CollisionSystem.h"
#include <cstddef>
#include <vector>

class PlayState : public State
{
public:
    explicit PlayState(Game& game);

    void init() override;
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    void initPlayers();
    void initInput();
    void initPlatforms();
    void initUI();

    void handlePlatformCollisions(Player& player);
    void handleScreenEdges(Player& player);
    void handleAttacks();
    void updateHealthBars();
    void checkGameOver();

    static constexpr std::size_t playerCount = 2;

    InputSystem inputSystem;
    CollisionSystem collisionSystem;

    std::vector<PlayerConfig> playerConfigs;
    std::vector<Player> players;
    std::vector<PlayerController> controllers;

    std::vector<sf::RectangleShape> platforms;

    std::vector<sf::RectangleShape> healthBars;
    float healthBarWidth = 0.f;
};