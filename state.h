#pragma once
#include "world.h"
#include "player.h"
#include "camera.h"

class state {
	virtual void update() {};
	virtual void fixedUpdate(bool hasFocus) {}; // 60 times pet second.

	virtual void redraw() {};
	virtual void draw(sf::RenderWindow& window) {};
};

class playState: state {
public:
	playState();
	World* world;
	Camera* camera;

	int incrementor = 0;
	std::vector<Player*> players;
	Player* me;
	void AddPlayer(Player& p);

	virtual void update() override;
	virtual void fixedUpdate(bool hasFocus) override;

	virtual void redraw() override;
	virtual void draw(sf::RenderWindow& window) override;
};