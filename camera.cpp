#include "camera.h"

Camera::Camera(World* wrld) {
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
	view.setCenter(center + sf::Vector2f(tilesize / 2, tilesize / 2));
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
	} else if(event.type == sf::Event::MouseButtonPressed) {
		if(event.mouseButton.button == sf::Mouse::Middle) {
			view.setSize(defaultZoom);
			window->setView(view);
		}
	}
}

void Camera::fixedUpdate(bool hasFocus) {
	if(following != nullptr) {
		center += (following->location - center) / 16.f;
		setCenter(center);
	}

	if(hasFocus) {
		sf::Vector2f worldPos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

		if(world->isLocInBounds(worldPos)) {
			sf::Vector2f worldcoords = sf::Vector2f(worldPos.x / tilesize, worldPos.y / tilesize);
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				world->placeBlock(1, worldcoords);
			}
			if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				world->placeBlock(0, worldcoords);
			}
		}
	}
}

sf::Vector2f Camera::getWorldLocation(sf::RenderWindow& window) {
	return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}
sf::Vector2f Camera::getWorldCoords(sf::RenderWindow& window) {
	sf::Vector2f worldPos = getWorldLocation(window);
	return sf::Vector2f(worldPos.x / tilesize, worldPos.y / tilesize);
}