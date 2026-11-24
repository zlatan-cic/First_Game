#include "Player.h"
#include <iostream>


// Members are listed in the same order as in Player.h
Player::Player(const PlayerConfig& config)
    : name(config.name),
    speed(config.speed),
    gravity(900.f),
    jumpForce(config.jumpForce),
    lives(config.lives),
    health(config.maxHealth),
    maxHealth(config.maxHealth),
    alive(true),
    movement(0.f, 0.f),
    velocity(0.f, 0.f),
    spawnPosition(config.startPosition),
    standardHeight(50.f),
    crouchingHeight(25.f),
    horizontalState(HorizontalPlayerState::Standing),
    verticalState(VerticalPlayerState::Falling)
{
    shape.setSize({ 50.f, standardHeight });
    shape.setFillColor(config.color);

    shape.setPosition(config.startPosition);
}

const std::string& Player::getName() const
{
    return name;
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
        facing = FacingDirection::Left;

        tryHorizontalStateTransition(
            HorizontalPlayerState::MovingLeft
        );
    }
}

void Player::moveRight()
{
    if (!isCrouching())
    {
        movement.x += speed;
        facing = FacingDirection::Right;

        tryHorizontalStateTransition(
            HorizontalPlayerState::MovingRight
        );
    }
}

void Player::jump()
{
    if (tryVerticalStateTransition(
        VerticalPlayerState::Jumping
    ))
    {
        velocity.y = jumpForce;
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
    // Count down until the next attack is allowed
    if (attackTimer > 0.f)
    {
        attackTimer -= dt;
    }

    // Gravity
    velocity.y += gravity * dt;

    if (velocity.y > 0.f &&
        (verticalState == VerticalPlayerState::Jumping ||
            verticalState == VerticalPlayerState::Standing))
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

void Player::landOn(float surfaceY)
{
    shape.setPosition({
        shape.getPosition().x,
        surfaceY - shape.getSize().y
        });

    velocity.y = 0.f;

    if (!isCrouching())
    {
        verticalState = VerticalPlayerState::Standing;
    }
}

void Player::setPositionX(float x)
{
    shape.setPosition({ x, shape.getPosition().y });
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
    if (!alive)
    {
        return;
    }

    lives--;
    std::cout << name << " lives: " << lives << '\n';

    if (lives > 0)
    {
        resetHealth();
        resetPosition();
    }
    else
    {
        alive = false;
        std::cout << name << " is out!\n";
    }
}

int Player::getLives() const
{
    return lives;
}

void Player::takeDamage(int damage)
{
    health -= damage;

    // The player decides what happens when HP runs out
    if (health <= 0)
    {
        health = 0;
        loseLife();
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

int Player::getMaxHealth() const
{
    return maxHealth;
}

void Player::attack()
{
    // Dead players can't attack, and not during cooldown
    if (!alive || attackTimer > 0.f)
    {
        return;
    }

    attacking = true;
    attackTimer = attackCooldown;
}

bool Player::isAttacking() const
{
    return attacking;
}

void Player::finishAttack()
{
    attacking = false;
}

sf::FloatRect Player::getAttackBounds() const
{
    const sf::FloatRect body = getBounds();

    // A box in front of the player, on the side it is facing
    const float x = (facing == FacingDirection::Right)
        ? body.position.x + body.size.x
        : body.position.x - attackRange;

    return sf::FloatRect({ x, body.position.y }, { attackRange, body.size.y });
}

int Player::getAttackDamage() const
{
    return attackDamage;
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

bool Player::isMovingDown() const
{
    return velocity.y >= 0.f;
}