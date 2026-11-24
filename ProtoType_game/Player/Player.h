#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "PlayerConfig.h"

enum class HorizontalPlayerState
{
    Standing,
    MovingLeft,
    MovingRight
};

enum class FacingDirection
{
    Left,
    Right
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
    std::string name;
    sf::RectangleShape shape;

    float speed;
    float gravity;
    float jumpForce;

    int lives;
    int health;
    int maxHealth;

    bool alive;

    sf::Vector2f movement;
    sf::Vector2f velocity;
    sf::Vector2f spawnPosition;

    float standardHeight;
    float crouchingHeight;

    HorizontalPlayerState horizontalState;
    VerticalPlayerState verticalState;

    //Attack: the player only knows ITS attack, not who it will hit
    FacingDirection facing = FacingDirection::Right;
    bool attacking = false;
    float attackTimer = 0.f;      // time left until the next attack
    float attackCooldown = 0.3f;   // pause between two attacks
    float attackRange = 40.f;
    int attackDamage = 10;

    bool tryVerticalStateTransition(VerticalPlayerState nextState);
    bool tryHorizontalStateTransition(HorizontalPlayerState nextState);

    bool isGrounded() const;
    bool isCrouching() const;

public:
    explicit Player(const PlayerConfig& config);

    const std::string& getName() const;

    void resetInput();

    void moveLeft();
    void moveRight();
    void jump();
    void startCrouch();
    void stopCrouch();
    void resetPosition();
    void loseLife();

    int getLives() const;
    void takeDamage(int damage);
    int getHealth() const;
    void resetHealth();
    int getMaxHealth() const;

    
    void attack();
    bool isAttacking() const;
    void finishAttack();
    sf::FloatRect getAttackBounds() const;
    int getAttackDamage() const;

    void setAlive(bool value);
    bool isAlive() const;

    void landOn(float surfaceY);
    void setPositionX(float x);

    bool isMovingDown() const;


    sf::FloatRect getBounds() const;

    sf::Vector2f getPosition() const;

    void update(float dt);
    void render(sf::RenderWindow& m_Window);

};