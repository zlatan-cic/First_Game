#pragma once
#include <SFML/Window/Keyboard.hpp>

struct PlayerControls
{
    sf::Keyboard::Key moveLeft;
    sf::Keyboard::Key moveRight;
    sf::Keyboard::Key jump;
    sf::Keyboard::Key crouch;
    sf::Keyboard::Key attack = sf::Keyboard::Key::Unknown; // Unknown = no attack key
}; 