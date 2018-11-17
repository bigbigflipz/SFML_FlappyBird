#pragma once

#include "SFML/Graphics/Rect.hpp"

static bool IsRectRectCollision(sf::IntRect r1, sf::IntRect r2)
{
	if (r1.left + r1.width >= r2.left &&     // r1 right edge past r2 left
		r1.left <= r2.left + r2.width &&       // r1 left edge past r2 right
		r1.top + r1.height >= r2.top &&       // r1 top edge past r2 bottom
		r1.top <= r2.top + r2.height) {       // r1 bottom edge past r2 top
		return true;
	}
	return false;
}