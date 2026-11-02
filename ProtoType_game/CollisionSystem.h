#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"

enum CollisionSide
{
	None,
	Top,
	Bottom,
	Left,
	Right
};

struct CollisionInfo
{
	bool hit = false;
	CollisionSide side = CollisionSide::None;
};

class CollisionSystem
{
public:
	void resolvePlatforms(Player& player, const std::vector<sf::RectangleShape>& platforms);

	CollisionInfo  check(const sf::FloatRect& a, const sf::FloatRect& b) const;
	
	

};
