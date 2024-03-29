#include "world.h"
#include "rayCasting.h"

World::World(int w = 25, int h = 25) : Map(w, h) {
	raysdraw = sf::VertexArray(sf::TriangleFan, 1);
};

void World::fixedUpdate(bool hasFocus) {
	Map::fixedUpdate(hasFocus);
}

void World::updateRays() {
	getPoints(*this, *(state->me)); //update rays
}

void World::update() {
	Map::update();
	updateRays(); //update rays
	redraw();
}

void World::redraw() {
	Map::redraw();

	edgesdraw.clear();
	for(auto line : edges) {
		edgesdraw.append(sf::Vertex(line->start, sf::Color::Green));
		edgesdraw.append(sf::Vertex(line->end, sf::Color::Green));
	}

	raysdraw.clear();
	raysdraw.append(sf::Vertex(state->me->center(), sf::Color(34, 139, 34, 100)));
	for(auto p : raypoints) {
		raysdraw.append(sf::Vertex(std::get<1>(p), sf::Color(34, 139, 34, 100)));
	}
}

void World::draw(sf::RenderWindow& window) {

	window.draw(raysdraw);
	Map::draw(window);


	//sf::CircleShape corner;
	//corner.setRadius(2);
	//for(auto line : edges) {
	//	corner.setFillColor(sf::Color(128, 0, 128));
	//	corner.setPosition(line->start - sf::Vector2f(corner.getRadius(), corner.getRadius()));
	//	window.draw(corner);
	//	//corner.setFillColor(sf::Color::Blue);
	//	//corner.setPosition(line->end - sf::Vector2f(corner.getRadius(), corner.getRadius()));
	//	//window.draw(corner);
	//}

	//corner.setFillColor(sf::Color(34, 139, 34));
	//for(auto rayp : raypoints) {
	//	corner.setPosition(sf::Vector2f(std::get<1>(rayp) - sf::Vector2f(corner.getRadius(), corner.getRadius())));
	//	window.draw(corner);
	//}
	//window.draw(edgesdraw);
};

