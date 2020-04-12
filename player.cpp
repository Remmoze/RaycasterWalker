#include "player.h"
#include "world.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>

Player::Player(const char* _name) {
	name = _name;
	id = -1;
	location = sf::Vector2f(.0f, .0f);
	color = sf::Color::Blue;
	world = nullptr;
};

void Player::say(const char* message) {
	printf("[%s]: %s\n", name, message);
};

sf::Vector2f Player::center() {
	return location + sf::Vector2f(tilesize / 2, tilesize / 2);
}

void Player::draw(sf::RenderWindow& window) {
	sf::CircleShape smiley;
	smiley.setRadius(tilesize / 2);
	smiley.setFillColor(color);
	smiley.setPosition(location);
	//printf("(%F, %f)\n", location.x, location.y);
	window.draw(smiley);
}

void Me::handleEvent(sf::Event event) {
	if(event.type == sf::Event::KeyPressed) {
		if(event.key.code == sf::Keyboard::Q) {
			color = color == sf::Color::Blue ? sf::Color::Red : sf::Color::Blue;
		}
	}
};

void Me::update(bool hasFocus) {
	if(hasFocus) {
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			location += sf::Vector2f(0, -tilesize / 4);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			location += sf::Vector2f(-tilesize / 4, 0);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			location += sf::Vector2f(0, tilesize / 4);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			location += sf::Vector2f(tilesize / 4, 0);

		location.x = fmaxf(fminf(location.x, (world->width - 1) * tilesize), 0);
		location.y = fmaxf(fminf(location.y, (world->height - 1) * tilesize), 0);
		world->update();
	}

	world->border.edgeAt[Up]->start.x = world->border.edgeAt[Left]->end.x = std::max(center().x - world->bordersize.x / 2, 0.0f);
	world->border.edgeAt[Up]->start.y = world->border.edgeAt[Left]->end.y = std::max(center().y - world->bordersize.y / 2, 0.0f);

	world->border.edgeAt[Right]->start.x = world->border.edgeAt[Up]->end.x = std::min(center().x + world->bordersize.x / 2, (float)world->width * tilesize);
	world->border.edgeAt[Right]->start.y = world->border.edgeAt[Up]->end.y = std::max(center().y - world->bordersize.y / 2, 0.0f);

	world->border.edgeAt[Down]->start.x = world->border.edgeAt[Right]->end.x = std::min(center().x + world->bordersize.x / 2, (float)world->width * tilesize);
	world->border.edgeAt[Down]->start.y = world->border.edgeAt[Right]->end.y = std::min(center().y + world->bordersize.y / 2, (float)world->height * tilesize);

	world->border.edgeAt[Left]->start.x = world->border.edgeAt[Down]->end.x = std::max(center().x - world->bordersize.x / 2, 0.0f);
	world->border.edgeAt[Left]->start.y = world->border.edgeAt[Down]->end.y = std::min(center().y + world->bordersize.y / 2, (float)world->height * tilesize);
}