#include "world.h"
#include "map.h"
#include <SFML/Graphics.hpp>
#include "vertexGenerator.h"
#include "rayCasting.h"


World::World(int w = 25, int h = 25) : Map(w, h) {
	edgesdraw = sf::VertexArray(sf::Lines, 2);
};

void World::redraw() {
	Map::redraw();
	raypoints = getPoints(*this, *players[0]);
	convert(*this);
	edgesdraw.clear();
	for(auto line : edges) {
		edgesdraw.append(sf::Vertex(line.start, sf::Color::Green));
		edgesdraw.append(sf::Vertex(line.end, sf::Color::Green));
	}

}

void World::draw(sf::RenderWindow& window) {
	window.setTitle("Edges: " + std::to_string(edges.size()) + " Points: " + std::to_string(raypoints.size()));

	sf::CircleShape smiley;
	smiley.setRadius(tilesize/2);
	for(int i = 0; i < players.size(); i++) {
		smiley.setFillColor((*players[i]).color);
		smiley.setPosition((*players[i]).location);
		window.draw(smiley);
	}

	Map::draw(window);


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
	
	sf::CircleShape corner;
	corner.setRadius(2);
	corner.setFillColor(sf::Color::Green);
	for(auto line : edges) {
		corner.setPosition(line.start - sf::Vector2f(corner.getRadius(), corner.getRadius()));
		window.draw(corner);
	}

	corner.setFillColor(sf::Color(34, 139, 34));
	for(auto rayp : raypoints) {
		corner.setPosition(rayp.pos - sf::Vector2f(corner.getRadius(), corner.getRadius()));
		window.draw(corner);
	}
	/*
	corner.setFillColor(sf::Color(34, 139, 34));
	if(edges.size() > 3) {
		auto poi = intersects((*players[0]).center(), edges[1].start, edges[0].start, edges[2].start);
		edgesdraw.clear();
		edgesdraw.append(sf::Vertex(edges[0].start, sf::Color::Green));
		edgesdraw.append(sf::Vertex(edges[2].start, sf::Color::Green));
		edgesdraw.append(sf::Vertex(edges[1].start, sf::Color::Green));
		edgesdraw.append(sf::Vertex((*players[0]).center(), sf::Color::Green));

		corner.setPosition(poi - sf::Vector2f(corner.getRadius(), corner.getRadius()));
		window.draw(corner);
		//printf("(%f, %f)\n", poi.x, poi.y);
	}
	*/

	window.draw(edgesdraw);
};

void World::AddPlayer(Player& p) {
	p.id = incrementor++;
	p.world = this;
	players.push_back(&p);
}