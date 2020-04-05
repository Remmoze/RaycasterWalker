#pragma once
#include "map.h"
#include "player.h"

#include "vertexGenerator.h"
struct Edge;

class World: public Map {
public:
	World(int w, int h);

	sf::VertexArray edgesdraw;
	std::vector<Edge> edges;

	sf::VertexArray raysdraw;
	std::vector<std::tuple<float, sf::Vector2f>> raypoints;

	void draw(sf::RenderWindow& window);
	void redraw();

	int incrementor = 0;
	std::vector<Player*> players;
	Player* me;
	void AddPlayer(Player& p);
};