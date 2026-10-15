#include "CollisionSystem.h"

void CollisionSystem::resolvePlatforms(Player& player, const std::vector<sf::RectangleShape>& platforms)
{
	for (const auto& platform : platforms)
	{
		const auto playerBounds = player.getBounds();
		const auto platformBounds = platform.getGlobalBounds();
		
		if (playerBounds.findIntersection(platformBounds))
		{
			if (player.isMovingDown() && player.getPreviousBottom() <= platformBounds.position.y)
			{
				player.landOn(platform.getPosition().y);
			}
		}
	}
}

