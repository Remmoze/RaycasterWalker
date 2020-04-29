#include <SFML/Graphics.hpp>
#include "state.h"

/*
todo:

use word "location" to refer to real co-ords (pixels), use floats
use word "Position" to refer to world co-ords (x,y), use ints

*/

#define printVecF(p) printf("(%f, %f)\n", p.x, p.y);

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 800), "2dplat", sf::Style::Default);

	playState state;
	state.camera->window = &window;

	sf::Clock timer;
	sf::Clock fps_clock;
	int fps_counter = 0;
	fps_clock.restart();

	constexpr unsigned TPS = 60; // ticks per seconds
	const sf::Time timePerUpdate = sf::seconds(1.0f / float(TPS));
	unsigned ticks = 0;

	auto lastTime = sf::Time::Zero;
	auto lag = sf::Time::Zero;

	state.update();

	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			((Me*)state.me)->handleEvent(event);
			state.camera->handleEvent(event);
			if(event.type == sf::Event::Closed)
				window.close();
			else if(event.type == sf::Event::KeyPressed) {
				if(event.key.code == sf::Keyboard::Z) {
					for(int i = 0; i < state.world->edges.size(); i++) {
						auto edge = state.world->edges[i];
						printf("[%i]: {%i|%f, %i|%f} -> {%i|%f, %i|%f}\n", i, (int)edge->start.x / 16, edge->start.x, (int)edge->start.y / 16, edge->start.y, (int)edge->end.x / 16, edge->end.x, (int)edge->end.y / 16, edge->end.y);
					}
					printf("\n");
				}
				else if(event.key.code == sf::Keyboard::K) {
				}
			}
		}


		// Get times
		auto time = timer.getElapsedTime();
		auto elapsed = time - lastTime;

		lastTime = time;
		lag += elapsed;

		bool focus = window.hasFocus();
		// Fixed time update
		while(lag >= timePerUpdate) {
			ticks++;
			lag -= timePerUpdate;

			state.fixedUpdate(focus);
		}

		window.clear();
		state.draw(window);
		window.display();

		if(fps_clock.getElapsedTime().asMilliseconds() >= 1000) {
			fps_clock.restart();
			window.setTitle(std::string("SFMLPlatformer -- FPS: " + std::to_string(fps_counter)));
			fps_counter = 0;
		}
		++fps_counter;
	}

	return 0;
}