#pragma once
#include "map.h"
#include "player.h"

#include "vertexGenerator.h"
struct Edge;

#include "rayCasting.h"
struct AnglePoint;


class World: public Map {
public:
	World(int w, int h);

	sf::VertexArray edgesdraw;
	std::vector<Edge> edges;

	std::vector<AnglePoint> raypoints;


	void draw(sf::RenderWindow& window);
	void redraw();

	int incrementor = 0;
	std::vector<Player*> players;
	void AddPlayer(Player& p);
};