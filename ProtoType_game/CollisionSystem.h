#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "CollisionSide.h"


struct CollisionInfo
{
	bool hit = false;
	CollisionSide side = CollisionSide::None;
};

class CollisionSystem
{
public:

	CollisionInfo  check(const sf::FloatRect& a, const sf::FloatRect& b) const;
	
	

};
