#pragma once
#include "map.h"
#include "player.h"

class World: public Map {
public:
	World(int w, int h);

	sf::VertexArray raysdraw;
	std::vector<std::tuple<float, sf::Vector2f>> raypoints;

	void update();
	void draw(sf::RenderWindow& window);
	void redraw();

	int incrementor = 0;
	std::vector<Player*> players;
	Player* me;
	void AddPlayer(Player& p);
};