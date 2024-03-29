#pragma once
#include <SFML/Graphics.hpp>
class World;//#include "world.h"

class Player {
public:
	Player(const char* name);
	const char* name;
	int id;

	World* world;
	virtual void fixedUpdate(bool hasFocus) {};

	sf::Vector2f location;
	sf::Vector2f center();

	sf::Color color;

	void draw(sf::RenderWindow& window);
	void say(const char* message);
};

class Me: public Player {
public:
	Me(const char* name) : Player(name) {};

	void handleEvent(sf::Event event);
	void fixedUpdate(bool hasFocus) override;
};