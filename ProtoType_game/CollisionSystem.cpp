#include "CollisionSystem.h"



CollisionInfo  CollisionSystem::check(const sf::FloatRect& a, const sf::FloatRect& b) const
{
	CollisionInfo  info;

	const auto overlap = a.findIntersection(b);

	if (!overlap)
	{
		return info;
	}

	info.hit = true;

	const float aCenterX = a.position.x + a.size.x / 2.f;
	const float aCenterY = a.position.y + a.size.y / 2.f;
	const float bCenterX = b.position.x + b.size.x / 2.f;
	const float bCenterY = b.position.y + b.size.y / 2.f;

	if (overlap->size.x > overlap->size.y)
	{
		if (aCenterY < bCenterY)
		{
			info.side = CollisionSide::Top;
		}
		else
		{
			info.side = CollisionSide::Bottom;
		}
	}
	else
	{
		if (aCenterX < bCenterX)
		{
			info.side = CollisionSide::Left;
		}
		else
		{
			info.side = CollisionSide::Right;
		}
	}

	return info;
}

ScreenEdgeInfo CollisionSystem::checkScreenEdges(const sf::FloatRect& shape, const sf::FloatRect& screen) const
{
	ScreenEdgeInfo info;

	const float shapeLeft = shape.position.x;
	const float shapeRight = shape.position.x + shape.size.x;
	const float shapeTop = shape.position.y;

	const float screenLeft = screen.position.x;
	const float screenRight = screen.position.x + screen.size.x;
	const float screenBottom = screen.position.y + screen.size.y;

	info.outLeft = shapeLeft < screenLeft;
	info.outRight = shapeRight > screenRight;
	info.outBottom = shapeTop > screenBottom;

	return info;
}