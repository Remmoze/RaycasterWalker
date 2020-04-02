#pragma once
#include <SFML/Graphics.hpp>
#define tilesize 16
#include "player.h"

struct Cell {
	int type = 0;
};

class World {
public:
	int width;
	int height;
	
	sf::VertexArray grid;
	Cell* cells;

	int incrementor = 0;
	std::vector<Player*> players;

	World(int w, int h);

	void draw(sf::RenderWindow& window);
	void placeBlock(int block, int x, int y);

	bool isInBounds(sf::Vector2f point);

	void AddPlayer(Player* p);
};