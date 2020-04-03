#pragma once
#include <SFML/Graphics.hpp>
//#include "world.h"
class World;

class Player {
public:
	const char* name;
	int id;

	sf::Color color;

	sf::Vector2f location;
	World* world;

	Player(const char* name);

	void say(const char* message);
};

class Me: public Player {
public:
	Me(const char* name) : Player(name) {};

	void handleEvent(sf::Event event);
	void update(bool hasFocus);
};