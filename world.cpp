#include "world.h"
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

	
	Map::draw(window); 
	for(auto p : players)
		(*p).draw(window);

	
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

	window.draw(edgesdraw);
};

void World::AddPlayer(Player& p) {
	p.id = incrementor++;
	p.world = this;
	players.push_back(&p);
}