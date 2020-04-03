#pragma once
#define tilesize 16

#include <SFML/Graphics.hpp>
#include "player.h"
#include "vertexGenerator.h"
struct Edge; //reeee

struct Cell {
	int type = 0;
};

class World {
public:
	int width;
	int height;
	
	sf::VertexArray grid;
	Cell* cells;

	sf::VertexArray edgesdraw;
	std::vector<Edge> edges;

	int incrementor = 0;
	std::vector<Player*> players;

	World(int w, int h);

	void draw(sf::RenderWindow& window);
	void redraw();
	void placeBlock(int block, int x, int y);
	void placeBlock(int block, sf::Vector2f loc);

	bool isInBounds(sf::Vector2f point);

	void AddPlayer(Player& p);
};