#include "world.h"
#include <SFML/Graphics.hpp>

World::World(int w = 25, int h = 25) {
	width = w;
	height = h;
	grid = sf::VertexArray(sf::Lines, 2);

	for(int x = 0; x <= width; x++) {
		grid.append(sf::Vertex(sf::Vector2f(x * tilesize, 0.f)));
		grid.append(sf::Vertex(sf::Vector2f(x * tilesize, height * tilesize)));
	};
	for(int y = 0; y <= height; y++) {
		grid.append(sf::Vertex(sf::Vector2f(0.f, y * tilesize)));
		grid.append(sf::Vertex(sf::Vector2f(width * tilesize, y * tilesize)));
	};

	cells = new Cell[width * height];
};

void World::draw(sf::RenderWindow& window) {
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(tilesize, tilesize));
	rect.setFillColor(sf::Color::Red);

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			if(cells[y * width + x].type == 1) {
				rect.setPosition(x * tilesize, y * tilesize);
				window.draw(rect);
			}
		}
	}

	for(int i = 0; i < players.size(); i++) {
		rect.setFillColor((*players[i]).color);
		rect.setPosition((*players[i]).location);
		window.draw(rect);
	}
	window.draw(grid);
};

bool World::isInBounds(sf::Vector2f point) {
	return point.x >= 0 && point.x < width*tilesize && point.y >= 0 && point.y < height * tilesize;
}

void World::placeBlock(int block, int x, int y) {
	int loc = y * width + x;
	if(cells[loc].type == block) return;
	cells[loc].type = block;
};
void World::placeBlock(int block, sf::Vector2f loc) {
	placeBlock(block, (int)loc.x, (int)loc.y);
};

void World::AddPlayer(Player& p) {
	p.id = incrementor++;
	p.world = this;
	players.push_back(&p);
}