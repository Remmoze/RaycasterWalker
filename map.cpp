#include "map.h"

Map::Map(int w = 25, int h = 25) {
	width = w;
	height = h;
	grid = sf::VertexArray(sf::Lines, 2);

	auto gridcolor = sf::Color(255, 255, 255, 100);
	for(int x = 0; x <= width; x++) {
		grid.append(sf::Vertex(sf::Vector2f(x * tilesize, 0.f), gridcolor));
		grid.append(sf::Vertex(sf::Vector2f(x * tilesize, height * tilesize), gridcolor));
	};
	for(int y = 0; y <= height; y++) {
		grid.append(sf::Vertex(sf::Vector2f(0.f, y * tilesize), gridcolor));
		grid.append(sf::Vertex(sf::Vector2f(width * tilesize, y * tilesize), gridcolor));
	};

	cells = new Cell[width * height];
};

void Map::placeBlock(int block, int x, int y) {
	int loc = y * width + x;
	if(cells[loc].type == block) return;
	cells[loc].type = block;

	update();
};
void Map::placeBlock(int block, sf::Vector2f loc) {
	placeBlock(block, (int)loc.x, (int)loc.y);
};

void Map::update() {

}

void Map::redraw() {

}

void Map::draw(sf::RenderWindow& window) {
	window.draw(grid);

	sf::RectangleShape tile;
	tile.setSize(sf::Vector2f(tilesize, tilesize));
	tile.setFillColor(sf::Color::Red);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			if(cells[y * width + x].type == 1) {
				tile.setPosition(x * tilesize, y * tilesize);
				window.draw(tile);
			}
		}
	}
}

bool Map::isInBounds(sf::Vector2f point) {
	return point.x >= 0 && point.x < width* tilesize && point.y >= 0 && point.y < height* tilesize;
}