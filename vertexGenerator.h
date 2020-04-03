#pragma once
#include <SFML/Graphics.hpp>
#include "world.h"

/*
convert tile map to the vector of vericies.
*/

/*
currently the edges vecter is reset entierly when the new block is placed
todo: modify the edges vector by running the calculation on placed/removed block, so only a small region get's replaced, not the whole vector
*/

struct Edge {
	sf::Vector2f start;
	sf::Vector2f end;
};

// special vector cell
struct vCell {
	int edgeId[4];
	bool edgeExists[4];
};

std::vector<Edge> convert(World& world);
std::vector<Edge> convert(World& world, sf::Vector2f start, sf::Vector2f size);
