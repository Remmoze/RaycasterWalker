#pragma once

#include "map.h"

#include <SFML/Graphics.hpp>
#include "player.h"

#include "vertexGenerator.h"
struct Edge;

#include "rayCasting.h"
struct AnglePoint;




class World: public Map {
public:
	sf::VertexArray edgesdraw;
	std::vector<Edge> edges;

	std::vector<AnglePoint> raypoints;

	int incrementor = 0;
	std::vector<Player*> players;
	World(int w, int h);

	void draw(sf::RenderWindow& window);
	void redraw();


	void AddPlayer(Player& p);
};