#include <SFML/Graphics.hpp>
#include "world.h"	
#include "camera.h"
#include "player.h"

#define printVecF(p) printf("(%f, %f)\n", p.x, p.y);

int main()
{
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

	float lala = 0.0000f;
	int i = 0;

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

				else if(event.key.code == sf::Keyboard::K) {
					for(auto p : world.raypoints) {
						printf("(%f)\n", std::get<0>(p));
					}
					printf("\n");
				}
			}

		}

		

		p.update(window.hasFocus());
		camera.update();

		window.clear();
		world.draw(window);
		window.display();
	}

	return 0;
}