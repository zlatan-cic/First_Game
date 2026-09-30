#include "Player.h"
#include <iostream>

Player::Player(sf::Vector2f startPosition, sf::Color color)
    : speed(250.f),
    gravity(900.f),
    jumpForce(-500.f),
    movement(0.f, 0.f),
    velocity(0.f, 0.f),
    standardHeight(50.f),
    crouchingHeight(25.f),
    horizontalState(HorizontalPlayerState::Standing),
    verticalState(VerticalPlayerState::Falling),
    spawnPosition(startPosition),
    lives(3),
    maxHealth(100),
    health(100),
    alive(true)
{
    shape.setSize({ 50.f, 50.f });
    shape.setFillColor(color);

    shape.setPosition(startPosition);
}

bool Player::isGrounded() const
{
    return verticalState == VerticalPlayerState::Crouching ||
        verticalState == VerticalPlayerState::Standing;
}

bool Player::isCrouching() const
{
    return verticalState == VerticalPlayerState::Crouching;
}

bool Player::tryVerticalStateTransition(VerticalPlayerState nextState)
{
    if (verticalState == VerticalPlayerState::Standing &&
        nextState == VerticalPlayerState::Jumping)
    {
        verticalState = nextState;
        return true;
    }
    else if (verticalState == VerticalPlayerState::Falling &&
        nextState == VerticalPlayerState::Standing)
    {
        verticalState = nextState;
        return true;
    }
    else if (verticalState == VerticalPlayerState::Jumping &&
        nextState == VerticalPlayerState::Falling)
    {
        verticalState = nextState;
        return true;
    }
    else if (verticalState == VerticalPlayerState::Standing &&
        nextState == VerticalPlayerState::Crouching)
    {
        verticalState = nextState;
        return true;
    }
    else if (verticalState == VerticalPlayerState::Crouching &&
        nextState == VerticalPlayerState::Standing)
    {
        verticalState = nextState;
        return true;
    }

    return false;
}

bool Player::tryHorizontalStateTransition(HorizontalPlayerState nextState)
{
    if (horizontalState == HorizontalPlayerState::Standing &&
        nextState == HorizontalPlayerState::MovingRight)
    {
        horizontalState = nextState;
        return true;
    }
    else if (horizontalState == HorizontalPlayerState::Standing &&
        nextState == HorizontalPlayerState::MovingLeft)
    {
        horizontalState = nextState;
        return true;
    }
    else if (horizontalState == HorizontalPlayerState::MovingLeft &&
        nextState == HorizontalPlayerState::Standing)
    {
        horizontalState = nextState;
        return true;
    }
    else if (horizontalState == HorizontalPlayerState::MovingRight &&
        nextState == HorizontalPlayerState::Standing)
    {
        horizontalState = nextState;
        return true;
    }

    return false;
}

void Player::moveLeft()
{
    if (!isCrouching())
    {
        movement.x -= speed;

        tryHorizontalStateTransition(
            HorizontalPlayerState::MovingLeft
        );

        std::cout << "MoveLeft!!!\n";
    }
}

void Player::moveRight()
{
    if (!isCrouching())
    {
        movement.x += speed;

        tryHorizontalStateTransition(
            HorizontalPlayerState::MovingRight
        );

        std::cout << "MoveRight!!!\n";
    }
}

void Player::jump()
{
    if (tryVerticalStateTransition(
        VerticalPlayerState::Jumping
    ))
    {
        velocity.y = jumpForce;

        std::cout << "State Jump!!!\n";
    }
}

void Player::startCrouch()
{
    if (tryVerticalStateTransition(
        VerticalPlayerState::Crouching
    ))
    {
        const float bottom =
            shape.getPosition().y +
            shape.getSize().y;

        shape.setSize({
            shape.getSize().x,
            crouchingHeight
            });

        shape.setPosition({
            shape.getPosition().x,
            bottom - crouchingHeight
            });

        std::cout << "CrouchStart!!!\n";
    }
}

void Player::stopCrouch()
{
    if (isCrouching())
    {
        if (tryVerticalStateTransition(
            VerticalPlayerState::Standing
        ))
        {
            const float bottom =
                shape.getPosition().y +
                shape.getSize().y;

            shape.setSize({
                shape.getSize().x,
                standardHeight
                });

            shape.setPosition({
                shape.getPosition().x,
                bottom - standardHeight
                });

            std::cout << "CrouchEnd!!!\n";
        }
    }
}

void Player::resetInput()
{
    movement.x = 0.f;

    tryHorizontalStateTransition(
        HorizontalPlayerState::Standing
    );
}

void Player::update(float dt)
{
    // Gravity
    velocity.y += gravity * dt;

    if (velocity.y > 0.f &&
        verticalState == VerticalPlayerState::Jumping)
    {
        tryVerticalStateTransition(
            VerticalPlayerState::Falling
        );
    }

    // Horizontal movement
    shape.move({
        movement.x * dt,
        0.f
    });

    // Vertical movement
    shape.move({
        0.f,
        velocity.y * dt
    });
}

void Player::resolvePlatformCollision(const sf::RectangleShape& platform)
{
    // Inportant!!!
    const auto playerBounds = shape.getGlobalBounds();
    const auto platformBounds = platform.getGlobalBounds();

    if (playerBounds.findIntersection(platformBounds))
    {
        if (velocity.y >= 0.f)
        {
            shape.setPosition({
                shape.getPosition().x,
                platform.getPosition().y - shape.getSize().y
            });

            velocity.y = 0.f;

            if (!isCrouching())
            {
                verticalState = VerticalPlayerState::Standing;
            }
        }
    }
}

void Player::resetPosition()
{
    shape.setPosition(spawnPosition);
    velocity = { 0.f,0.f };
    movement = { 0.f,0.f };

    horizontalState = HorizontalPlayerState::Standing;
    verticalState = VerticalPlayerState::Falling;
}

sf::Vector2f Player::getPosition() const
{
    return shape.getPosition();
}

void Player::loseLife()
{
    if (lives > 0)
    {
        lives--;
    }
}

int Player::getLives() const
{
    return lives;
}

void Player::takeDamage(int damage)
{
    health -= damage;

    if (health < 0)
    {
        health = 0;
    }
}

int Player::getHealth() const
{
    return health;
}

void Player::resetHealth()
{
    health = maxHealth;
}

sf::FloatRect Player::getBounds() const
{
    return shape.getGlobalBounds();
}

void Player::setAlive(bool value)
{
    alive = value;
}

bool Player::isAlive() const
{
    return alive;
}

void Player::render(sf::RenderWindow& m_Window)
{
    m_Window.draw(shape);
}