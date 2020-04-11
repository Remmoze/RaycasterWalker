#include "rayCasting.h"
#define PI 3.14159265358979323846

bool inBounds(char quadrant, sf::Vector2f start, Edge edge) {
	switch(quadrant) {
		case 1: return (edge.start.x >= start.x && edge.start.y <= start.y) || (edge.end.x >= start.x && edge.end.y <= start.y);
		case 2: return (edge.start.x <= start.x&& edge.start.y <= start.y) || (edge.end.x <= start.x&& edge.end.y <= start.y);
		case 3: return (edge.start.x <= start.x&& edge.start.y >= start.y) || (edge.end.x <= start.x&& edge.end.y >= start.y);
		case 4: return (edge.start.x >= start.x && edge.start.y >= start.y) || (edge.end.x >= start.x && edge.end.y >= start.y);
		default: {
			printf("invalid quadrant provided: %i", (int)quadrant);
			return false;
		}
	}
}

inline static bool angleCompare(const std::tuple<float, sf::Vector2f>& a, const std::tuple<float, sf::Vector2f>& b) {
	return std::get<0>(a) < std::get<0>(b);
}

void getPoints(World& world, Player& player) {
	int raystotal = 0;
	world.raypoints.clear();
	for(auto edge : world.edges) {
		sf::Vector2f endp = edge->start - player.center();
		float bangle = atan2f(endp.y, endp.x); //base angle from player to the point
		float ang = .0f;
		for(int i = 0; i < 3; i++) {
			ang = bangle + 0.0001f * (i - 1);
			endp = sf::Vector2f(cosf(ang), sinf(ang));
			float min_dist = INFINITY;
			std::tuple<float, sf::Vector2f> interpoint;
			bool valid = false;
			char quadrant;
			if(abs(ang) > PI / 2) {
				if(ang > 0) quadrant = 3;
				else quadrant = 2;
			} else {
				if(ang > 0) quadrant = 4;
				else quadrant = 1;
			}
			for(auto e2 : world.edges) {
				if(!inBounds(quadrant, player.center(), *e2)) continue;
				raystotal++;
				auto p1 = player.center();
				auto p2 = player.center() + endp;
				auto p3 = e2->start;
				auto p4 = e2->end;

				auto denom = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
				if(denom == 0) continue; //parallel

				auto t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / denom;
				auto u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / denom;

				if(u < 0 || u > 1) continue; //they don't collide
				if(t < 0) continue; //intersection on the opposite side
				if(t >= min_dist) continue; // we already found a shorter ray
				min_dist = t;

				auto x = p1.x + t * (p2.x - p1.x);
				auto y = p1.y + t * (p2.y - p1.y);
				interpoint = {atan2f(y - player.center().y, x - player.center().x), {x, y}};
				valid = true;
			}
			if(valid)
				world.raypoints.push_back(interpoint);
		}
	}
	std::sort(world.raypoints.begin(), world.raypoints.end(), angleCompare);
	if(world.raypoints.size() > 0)
		world.raypoints.push_back(world.raypoints[0]);
	printf("rays: %i\n", raystotal);
}