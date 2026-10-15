#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"

class CollisionSystem
{
public:
	void resolvePlatforms(Player& player,
		const std::vector<sf::RectangleShape>& platforms);

};
