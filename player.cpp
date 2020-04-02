#include "player.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "world.h"

Player::Player(const char* _name) {
	name = _name;
	id = -1;
	location = sf::Vector2f(0, 0);
};

void Player::say(const char* message) {
	printf("[%s]: %s\n", name, message);
};

void Me::handleEvent(sf::Event event) {
	if(event.type == sf::Event::KeyPressed) {
		switch(event.key.code) {
			case sf::Keyboard::W: location += sf::Vector2f(0, -tilesize); break;
			case sf::Keyboard::A: location += sf::Vector2f(-tilesize, 0); break;
			case sf::Keyboard::S: location += sf::Vector2f(0, tilesize); break;
			case sf::Keyboard::D: location += sf::Vector2f(tilesize, 0); break;
		}
	}
};