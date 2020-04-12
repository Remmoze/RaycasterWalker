#pragma once
#define tilesize 16
#include <SFML/Graphics.hpp>

struct Edge {
	sf::Vector2f start;
	sf::Vector2f end;
};

// special vector cell
struct CellEdges {
	Edge* edgeAt[4];
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
	std::vector<Edge*> edges;
	CellEdges border;
	sf::Vector2f bordersize = {300, 300};

	void update();
	void redraw();
	void draw(sf::RenderWindow& window);

	bool isLocInBounds(sf::Vector2f point);
	bool isPosInBounds(sf::Vector2f point);

	void placeBlock(int block, int x, int y);
	void placeBlock(int block, sf::Vector2f loc);

private: 
	void calculateEdges(sf::Vector2i blockpos);
	void createEdge(sf::Vector2f blockloc, int cur, int edgeType);
	void deleteEdge(Edge* edge);

	void cutEdge(sf::Vector2f bpos, int edgeType);
	void wireEdge(sf::Vector2f bpos, int edgeType);

	//deprecated
	void recalculateEdges();
	
};

namespace {
	enum {
		Up, Right, Down, Left
	};
}