#pragma once
#include <SFML/Graphics.hpp>
#include "world.h"

class Camera {
private:
	sf::Vector2f center;
	sf::Vector2f defaultZoom;
	
	Player* following;
public:
	sf::RenderWindow* window;
	World* world;
	sf::View view;
	Camera(sf::RenderWindow* win, World* world);

	void handleEvent(sf::Event);

	sf::Vector2f getCenter();
	void setCenter(float x, float y);
	void setCenter(sf::Vector2f point);
	void move(float x, float y);
	void move(sf::Vector2f point);

	void follow(Player* p);

	void update();
};