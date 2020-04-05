#pragma once
#include <SFML/Graphics.hpp>
#include "world.h"

struct AnglePoint {
	float angle;
	sf::Vector2f pos;
};


std::vector<AnglePoint> getPoints(World& world, Player& player);
std::vector<AnglePoint> getPolygon(World& world, Player& player);

