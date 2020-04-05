#pragma once
#define tilesize 16
#include <SFML/Graphics.hpp>

struct Cell {
	int type = 0;
};

class Map {
public:
	Map(int w, int h);
	int width;
	int height;

	sf::VertexArray grid;
	Cell* cells;

	void update();
	void redraw();
	void draw(sf::RenderWindow& window);
	bool isInBounds(sf::Vector2f point);

	void placeBlock(int block, int x, int y);
	void placeBlock(int block, sf::Vector2f loc);
};

