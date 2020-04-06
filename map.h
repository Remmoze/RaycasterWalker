#pragma once
#define tilesize 16
#include <SFML/Graphics.hpp>

struct Edge {
	sf::Vector2f start;
	sf::Vector2f end;
};

// special vector cell
struct CellEdges {
	int edgeId[4];
	bool edgeExists[4];
};

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

	CellEdges* edgecells;
	sf::VertexArray edgesdraw;
	std::vector<Edge> edges;

	void update();
	void redraw();
	void draw(sf::RenderWindow& window);
	bool isInBounds(sf::Vector2f point);

	void placeBlock(int block, int x, int y);
	void placeBlock(int block, sf::Vector2f loc);
	void recalculateEdges();
	void cutEdge(int edgeType, CellEdges cell, sf::Vector2f bpos, int index);

private: 
	void calculateEdges(sf::Vector2f blockloc);
};

namespace {
	enum {
		Up, Right, Down, Left
	};
}