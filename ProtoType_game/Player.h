#pragma once
#include <SFML/Graphics.hpp>

enum class HorizontalPlayerState
{
    Standing,
    MovingLeft,
    MovingRight
};

enum class VerticalPlayerState
{
    Standing,
    Jumping,
    Falling,
    Crouching
};

class Player
{
private:
    sf::RectangleShape shape;

    float speed;
    float gravity;
    float jumpForce;

    

    sf::Vector2f movement;
    sf::Vector2f velocity;
    sf::Vector2f spawnPosition;
    

    float standardHeight;
    float crouchingHeight;

    HorizontalPlayerState horizontalState;
    VerticalPlayerState verticalState;

    bool tryVerticalStateTransition(VerticalPlayerState nextState);
    bool tryHorizontalStateTransition(HorizontalPlayerState nextState);

    bool isGrounded() const;
    bool isCrouching() const;

public:
    Player(sf::Vector2f startPosition, sf::Color color);

    void resetInput();

    void moveLeft();
    void moveRight();
    void jump();
    void startCrouch();
    void stopCrouch();
    void resetPosition();

    sf::Vector2f getPosition() const;

    void update(float dt);
    void render(sf::RenderWindow& m_Window);
    void resolvePlatformCollision(const sf::RectangleShape& platforms);
};