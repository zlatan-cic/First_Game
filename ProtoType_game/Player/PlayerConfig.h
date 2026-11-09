#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Podaci koji opisuju jednog igraca.
// Citaju se iz players.json, a Player ih dobija u konstruktoru.
// Vrednosti posle = su podrazumevane, koriste se ako nesto fali u JSON-u.
struct PlayerConfig
{
    std::string name = "Player";
    sf::Vector2f startPosition = { 0.f, 0.f };
    sf::Color color = sf::Color::White;

    float speed = 250.f;
    float jumpForce = -500.f; // negativno = gore (u SFML-u y raste nanize)

    int lives = 3;
    int maxHealth = 100;
};
