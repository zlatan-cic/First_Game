#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "CollisionSide.h"


struct CollisionInfo
{
	bool hit = false;
	CollisionSide side = CollisionSide::None;
};

struct ScreenEdgeInfo
{
	bool outLeft = false;
	bool outRight= false;
	bool outBottom = false;

};

class CollisionSystem
{
public:

	CollisionInfo  check(const sf::FloatRect& a, const sf::FloatRect& b) const;
	ScreenEdgeInfo checkScreenEdges(const sf::FloatRect& shape, const sf::FloatRect& screen) const;
};


