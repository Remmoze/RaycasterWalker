#include "state.h"

playState::playState() {
	World _world(25, 25);
	_world.state = this;
	world = &_world;

	Me p("gosha");
	p.location = sf::Vector2f(40.f, 80.f);
	AddPlayer(p);

	Player p2("jesse");
	p2.location += sf::Vector2f(10.f * tilesize, 10.f * tilesize);
	AddPlayer(p2);

	Camera camera(world);
	camera.follow(me);
};

void playState::AddPlayer(Player& p) {
	if(incrementor == 0)
		me = &p;
	p.id = incrementor++;
	p.world = world;
	players.push_back(&p);
}

void playState::update() {
	world->update();
}

void playState::fixedUpdate(bool hasFocus) {
	for(auto p : players)
		p->fixedUpdate(hasFocus);
	camera->fixedUpdate(hasFocus);
}

void playState::redraw() {
	world->redraw();
}

void playState::draw(sf::RenderWindow& window) {
	world->draw(window);
	for(auto p : players)
		p->draw(window);
}