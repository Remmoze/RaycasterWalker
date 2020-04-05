#include "world.h"
#include "vertexGenerator.h"
#include "rayCasting.h"

World::World(int w = 25, int h = 25) : Map(w, h) {
	edgesdraw = sf::VertexArray(sf::Lines, 2);
	raysdraw = sf::VertexArray(sf::TriangleFan, 1);
};

void World::redraw() {
	Map::redraw();

	convert(*this);

	Edge edge;
	edge.start = sf::Vector2f(0, 0);
	edge.end = sf::Vector2f(tilesize * width, 0);
	edges.push_back(edge);
	edge.start = sf::Vector2f(tilesize * width, 0);
	edge.end = sf::Vector2f(tilesize * width, tilesize * height);
	edges.push_back(edge);
	edge.start = sf::Vector2f(tilesize * width, tilesize * height);
	edge.end = sf::Vector2f(0, tilesize * height);
	edges.push_back(edge);
	edge.start = sf::Vector2f(0, tilesize * height);
	edge.end = sf::Vector2f(0, 0);
	edges.push_back(edge);



	getPoints(*this, *me);


	edgesdraw.clear();


	for(auto line : edges) {
		edgesdraw.append(sf::Vertex(line.start, sf::Color::Green));
		edgesdraw.append(sf::Vertex(line.end, sf::Color::Green));
	}

	raysdraw.clear();
	raysdraw.append(me->center());
	for(auto p : raypoints) {
		raysdraw.append(sf::Vertex(std::get<1>(p), sf::Color(34, 139, 34)));
	}

}

void World::draw(sf::RenderWindow& window) {
	window.setTitle("Edges: " + std::to_string(edges.size()));

	window.draw(raysdraw);

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
		corner.setPosition(sf::Vector2f(std::get<1>(rayp) - sf::Vector2f(corner.getRadius(), corner.getRadius())));
		window.draw(corner);
	}
	window.draw(edgesdraw);
};

void World::AddPlayer(Player& p) {
	if(incrementor == 0)
		me = &p;
	p.id = incrementor++;
	p.world = this;
	players.push_back(&p);
}