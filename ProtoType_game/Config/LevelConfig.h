#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// One platform in a level
struct PlatformConfig
{
    sf::Vector2f position = { 0.f, 0.f };
    sf::Vector2f size = { 100.f, 20.f };
};

// Everything that makes one level different: name, colors, platforms.
// Read from levels.json, PlayState builds the level from this in init().
struct LevelConfig
{
    std::string name = "Classic";
    sf::Color background = sf::Color::Black;
    sf::Color platformColor = sf::Color::Green;
    std::vector<PlatformConfig> platforms;
};