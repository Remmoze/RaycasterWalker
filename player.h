#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
	const char* name;
	int id;

	sf::Color color;

	sf::Vector2f location;

	Player(const char* name);

	void say(const char* message);
};

class Me: public Player {
public:
	Me(const char* name) : Player(name) {};

	void handleEvent(sf::Event event);
	void update();
};