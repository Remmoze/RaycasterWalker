#include <SFML/Graphics.hpp>
#include "camera.h"
#include "world.h"

Camera::Camera(sf::RenderWindow* win, World* wrld) {
	window = win;
	world = wrld;
	center = sf::Vector2f(8.0f, 8.0f);
	defaultZoom = sf::Vector2f(world->width * tilesize, world->height * tilesize);
	following = nullptr;

	view = sf::View(center, defaultZoom);
	window->setView(view);
};

sf::Vector2f Camera::getCenter() { return center; };
void Camera::setCenter(float x, float y) { setCenter(sf::Vector2f(x, y)); };
void Camera::setCenter(sf::Vector2f point) {
	center = point;
	view.setCenter(center + sf::Vector2f(tilesize/2, tilesize / 2));
	window->setView(view);
};

void Camera::move(float x, float y) { Camera::move(sf::Vector2f(x, y)); };
void Camera::move(sf::Vector2f point) {
	center += point;
	setCenter(center);
}

void Camera::follow(Player* p) {
	following = p;
}

void Camera::handleEvent(sf::Event event) {
	
	if(event.type == sf::Event::MouseWheelScrolled) {
		view.zoom(event.mouseWheelScroll.delta == -1 ? 2 : 0.5);
		window->setView(view);
	}

	if(event.type != sf::Event::MouseButtonPressed && event.type != sf::Event::MouseMoved) return;

	sf::Vector2f worldPos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	if(worldPos.x < 0 || worldPos.y < 0 || worldPos.x >= world->width * tilesize || worldPos.y >= world->height * tilesize)
		return;

	switch(event.type) {
		case sf::Event::MouseWheelScrolled: {
		} break;
		case sf::Event::MouseButtonPressed: {
			switch(event.mouseButton.button) {
				case sf::Mouse::Middle: {
					view.setSize(defaultZoom);
					window->setView(view);

				} break;
				case sf::Mouse::Left: {
					world->placeBlock(1, worldPos.x, worldPos.y);
				} break;
				case sf::Mouse::Right: {
					world->placeBlock(0, worldPos.x, worldPos.y);
				} break;
			}
		} break;
		case sf::Event::MouseMoved: {
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				world->placeBlock(1, worldPos.x, worldPos.y);
			}
			if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				world->placeBlock(0, worldPos.x, worldPos.y);
			}
		} break;
	}
}

void Camera::update() {
	if(following != nullptr) {
		center += (following->location - center) / 16.f;
		setCenter(center);
	}
}