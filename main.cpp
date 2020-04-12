#include <SFML/Graphics.hpp>
#include "world.h"	
#include "camera.h"
#include "player.h"

/*
todo:

use word "location" to refer to real co-ords (pixels), use floats
use word "Position" to refer to world co-ords (x,y), use ints

*/

#define printVecF(p) printf("(%f, %f)\n", p.x, p.y);

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 800), "2dplat", sf::Style::Default);

	World world(25, 25);
	Camera camera(&window, &world);

	Me p("gosha");
	p.location = sf::Vector2f(40.f, 80.f);
	world.AddPlayer(p);

	Player p2("jesse");
	p2.location += sf::Vector2f(10.f * tilesize, 10.f * tilesize);
	world.AddPlayer(p2);

	camera.follow(&p);
	window.setFramerateLimit(60);

	world.update();

	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			p.handleEvent(event);
			camera.handleEvent(event);
			if(event.type == sf::Event::Closed)
				window.close();
			else if(event.type == sf::Event::KeyPressed) {

				if(event.key.code == sf::Keyboard::P)
					camera.follow(&p2);
				else if(event.key.code == sf::Keyboard::O)
					camera.follow(&p);
				else if(event.key.code == sf::Keyboard::Z) {
					for(int i = 0; i < world.edges.size(); i++) {
						auto edge = world.edges[i];
						printf("[%i]: {%i|%f, %i|%f} -> {%i|%f, %i|%f}\n", i, (int)edge->start.x / 16, edge->start.x, (int)edge->start.y / 16, edge->start.y, (int)edge->end.x / 16, edge->end.x, (int)edge->end.y / 16, edge->end.y);
					}
					printf("\n");
				}
				else if(event.key.code == sf::Keyboard::K) {
				}
			}
		}
		bool focus = window.hasFocus();
		world.tick(focus);
		camera.tick(focus);

		window.clear();
		world.draw(window);
		window.display();
	}

	return 0;
}