#include <SFML/Graphics.hpp>
#include "world.h"	
#include "camera.h"
#include "player.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "2dplat");

	World world(25, 25);
	Camera camera(&window, &world);

	Me p("gosha");
	world.AddPlayer(&p);
	camera.follow(&p);
	window.setFramerateLimit(60);
	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			p.handleEvent(event);
			camera.handleEvent(event);
			if(event.type == sf::Event::Closed)
				window.close();
		}

		p.update();
		camera.update();

		window.clear();
		world.draw(window);
		window.display();
	}
	
	return 0;
}