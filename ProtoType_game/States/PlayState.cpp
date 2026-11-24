#include "PlayState.h"
#include "Game.h"
#include "ConfigLoader.h"
#include "PauseState.h"
#include "GameOver.h"
#include <iostream>
#include <string>

PlayState::PlayState(Game& game, const LevelConfig& level)
    : State(game),
    level(level)
{}

void PlayState::init()
{
    initPlayers();
    initInput();
    initPlatforms();
    initUI();
}

void PlayState::initPlayers()
{
    playerConfigs = loadPlayerConfigs("players.json", playerCount);

    players.clear();
    controllers.clear();

    players.reserve(playerConfigs.size());
    for (const PlayerConfig& config : playerConfigs)
    {
        players.emplace_back(config);
    }

    // Controllers only AFTER all players, because they hold a reference to a player
    controllers.reserve(players.size());
    for (Player& player : players)
    {
        controllers.emplace_back(player);
    }
}

void PlayState::initInput()
{
    // Key layouts: moveLeft, moveRight, jump, crouch, attack
    const std::vector<PlayerControls> keyboardLayouts = {
        {
            sf::Keyboard::Key::A,
            sf::Keyboard::Key::D,
            sf::Keyboard::Key::W,
            sf::Keyboard::Key::S,
            sf::Keyboard::Key::F
        },
        {
            sf::Keyboard::Key::Left,
            sf::Keyboard::Key::Right,
            sf::Keyboard::Key::Up,
            sf::Keyboard::Key::Down,
            sf::Keyboard::Key::RControl
        }
    };

    // A player without a layout has no keyboard (AI will control it later)
    for (std::size_t i = 0; i < controllers.size() && i < keyboardLayouts.size(); ++i)
    {
        inputSystem.addPlayer(
            keyboardLayouts[i],
            [this, i](PlayerAction action)
            {
                controllers[i].handleAction(action);
            }
        );
    }
}

void PlayState::initPlatforms()
{
    // Background fills the whole window in the level's color
    background.setSize(sf::Vector2f(game.getWindow().getSize()));
    background.setFillColor(level.background);

    // Platforms come from the level, not from code
    platforms.clear();

    for (const PlatformConfig& platformConfig : level.platforms)
    {
        sf::RectangleShape platform(platformConfig.size);
        platform.setPosition(platformConfig.position);
        platform.setFillColor(level.platformColor);
        platforms.push_back(platform);
    }
}

void PlayState::initUI()
{
    healthBars.clear();

    const float margin = 20.f;
    const float windowWidth = static_cast<float>(game.getWindow().getSize().x);
    const float count = static_cast<float>(players.size());

    // All bars fit in one row, no matter how many players
    healthBarWidth = (windowWidth - margin * (count + 1.f)) / count;

    for (std::size_t i = 0; i < players.size(); ++i)
    {
        sf::RectangleShape bar({ healthBarWidth, 20.f });
        bar.setPosition({ margin + static_cast<float>(i) * (healthBarWidth + margin), 20.f });
        bar.setFillColor(playerConfigs[i].color);
        healthBars.push_back(bar);
    }
}

void PlayState::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::Escape)
        {
            game.pushState(std::make_unique<PauseState>(game));
        }
    }
}

void PlayState::update(float dt)
{
    // Reset input from previous frame
    for (Player& player : players)
    {
        player.resetInput();
    }

    // Read keyboard
    inputSystem.update();

    for (Player& player : players)
    {
        if (!player.isAlive())
        {
            continue;
        }

        player.update(dt);
        handlePlatformCollisions(player);
        handleScreenEdges(player);
    }

    handleAttacks();
    updateHealthBars();
    checkGameOver();
}

void PlayState::handlePlatformCollisions(Player& player)
{
    for (const auto& platform : platforms)
    {
        const CollisionInfo info = collisionSystem.check(player.getBounds(), platform.getGlobalBounds());

        if (info.side == CollisionSide::Top && player.isMovingDown())
        {
            player.landOn(platform.getPosition().y);
        }
    }
}

void PlayState::handleScreenEdges(Player& player)
{
    const sf::FloatRect screen({ 0.f, 0.f }, sf::Vector2f(game.getWindow().getSize()));
    const ScreenEdgeInfo edges = collisionSystem.checkScreenEdges(player.getBounds(), screen);

    if (edges.outLeft)
    {
        player.setPositionX(screen.position.x);
    }

    if (edges.outRight)
    {
        player.setPositionX(screen.size.x - player.getBounds().size.x);
    }

    if (edges.outBottom)
    {
        // The player knows what losing a life means
        player.loseLife();
    }
}

void PlayState::handleAttacks()
{
    // It doesn't matter WHO attacks: every attacking player checks all others
    for (Player& attacker : players)
    {
        if (!attacker.isAlive() || !attacker.isAttacking())
        {
            continue;
        }

        const sf::FloatRect attackBox = attacker.getAttackBounds();

        for (Player& target : players)
        {
            // You can't hit yourself, and dead players don't count
            if (&target == &attacker || !target.isAlive())
            {
                continue;
            }

            if (collisionSystem.check(attackBox, target.getBounds()).hit)
            {
                target.takeDamage(attacker.getAttackDamage());

                std::cout << attacker.getName() << " hit " << target.getName()
                    << ", HP: " << target.getHealth() << '\n';
            }
        }

        attacker.finishAttack();
    }
}

void PlayState::updateHealthBars()
{
    for (std::size_t i = 0; i < players.size(); ++i)
    {
        const float ratio = static_cast<float>(players[i].getHealth()) /
            static_cast<float>(players[i].getMaxHealth());

        healthBars[i].setSize({ healthBarWidth * ratio, 20.f });
    }
}

void PlayState::checkGameOver()
{
    int aliveCount = 0;
    const Player* lastAlive = nullptr;

    for (const Player& player : players)
    {
        if (player.isAlive())
        {
            ++aliveCount;
            lastAlive = &player;
        }
    }

    // The game goes on while at least two players are alive
    if (aliveCount > 1)
    {
        return;
    }

    const std::string winnerName = lastAlive ? lastAlive->getName() : "Nobody";
    std::cout << "Winner: " << winnerName << '\n';

    game.pushState(std::make_unique<GameOverState>(game, winnerName));
}

void PlayState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    for (const auto& platform : platforms)
    {
        window.draw(platform);
    }

    for (Player& player : players)
    {
        if (player.isAlive())
        {
            player.render(window);
        }
    }

    for (const auto& bar : healthBars)
    {
        window.draw(bar);
    }
}