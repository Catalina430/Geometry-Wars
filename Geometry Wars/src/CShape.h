#pragma once

#include "Common.h"

class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		: circle(radius, points) 
	{
		circle.setOutlineColor(outline);
		circle.setFillColor(fill);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}

};

