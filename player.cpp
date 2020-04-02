#include "player.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "world.h"

Player::Player(const char* _name) {
	name = _name;
	id = -1;
	location = sf::Vector2f(.0f, .0f);
	color = sf::Color::Blue;
};

void Player::say(const char* message) {
	printf("[%s]: %s\n", name, message);
};

void Me::handleEvent(sf::Event event) {
	if(event.type == sf::Event::KeyPressed) {
		if(event.key.code == sf::Keyboard::Q) {
			color = color == sf::Color::Blue ? sf::Color::Red : sf::Color::Blue;
		}
	}
};

void Me::update() {
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		location += sf::Vector2f(0, -tilesize / 4);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		location += sf::Vector2f(-tilesize / 4, 0);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		location += sf::Vector2f(0, tilesize / 4);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		location += sf::Vector2f(tilesize / 4, 0);
}