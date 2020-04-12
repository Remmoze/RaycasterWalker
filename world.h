#pragma once
#include "map.h"
#include "player.h"

class World: public Map {
public:
	World(int w, int h);

	sf::VertexArray raysdraw;
	std::vector<std::tuple<float, sf::Vector2f>> raypoints;
	
	void tick(bool hasFocus) override; //called every frame
	void update() override; //called when update for certain parts is needed
	void updateRays();

	void redraw() override;
	void draw(sf::RenderWindow& window) override;

	int incrementor = 0;
	std::vector<Player*> players;
	Player* me;
	void AddPlayer(Player& p);
};