#pragma once
#include "map.h"
#include "state.h"

class World: public Map {
public:
	World(int w, int h);

	playState* state;

	sf::VertexArray raysdraw;
	std::vector<std::tuple<float, sf::Vector2f>> raypoints;
	
	void update() override; //called when update for certain parts is needed
	void fixedUpdate(bool hasFocus) override; //called every frame
	void updateRays();

	void redraw() override;
	void draw(sf::RenderWindow& window) override;

};