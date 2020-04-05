#include "rayCasting.h"
#include "world.h"

inline static bool angleCompare(const AnglePoint& a, const AnglePoint& b)
{
	return a.angle < b.angle;
}

std::vector<AnglePoint> getPoints(World& world, Player& player) {
	std::vector<AnglePoint> points;

	float maxDistance = tilesize * 80; // radius of 40

	for(Edge edge : world.edges) {
		sf::Vector2f rd = edge.start - player.center();

		float bangle = atan2f(rd.y, rd.x);
		float ang = .0f;
		for(int i = 0; i < 3; i++) {
			ang = bangle + 0.0001f * (i - 1);

			rd = sf::Vector2f(cosf(ang), sinf(ang)) * maxDistance;
			float min_t1 = INFINITY;
			AnglePoint point = {0, {0, 0}};
			bool valid = false;
			//optimize this so it only checks for edges in a correct (mathematical) plane.
			for(auto e2 : world.edges) {
				auto p1 = player.center();
				auto p2 = rd;
				auto p3 = edge.start;
				auto p4 = edge.end;

				auto tnum = (p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x);
				if(tnum == 0) continue;


				auto unum = (p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x);
				auto denom = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
				if(tnum == 0) continue;

				auto t = tnum / denom;
				auto u = unum / denom;

				if(t < 0 || t > 1 || u < -1 || u > 0) continue; //no intersection

				point.pos = {p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y)};
				valid = true;
			}
			if(valid)// Add intersection point to visibility polygon perimeter
				points.push_back(point);
		}
	}
	//std::sort(points.begin(), points.end(), angleCompare);
	return points;
};

/*
std::vector<AnglePoint> getPoints(World& world, Player& player) {
	std::vector<AnglePoint> points;

	float maxDistance = tilesize * 80; // radius of 40

	for(Edge edge : world.edges) {
		sf::Vector2f rd = edge.start - player.center();

		float bangle = atan2f(rd.y, rd.x);
		float ang = .0f;
		for(int i = 0; i < 3; i++) {
			ang = bangle + 0.0001f * (i-1);

			rd = sf::Vector2f(cosf(ang), sinf(ang)) * maxDistance;
			float min_t1 = INFINITY;
			AnglePoint p = {0, {0, 0}};
			bool valid = false;

			//optimize this so it only checks for edges in a correct (mathematical) plane.
			for(auto edge2 : world.edges) {
				sf::Vector2f sd = edge2.end - edge2.start;

				if(fabs(sd.x - rd.x) == 0.0f || fabs(sd.y - rd.y) == 0.0f) continue; //colleniar

				float t2 = (rd.x * (edge2.end.y - player.center().y) + (rd.y * (player.center().x - edge2.start.x))) / (sd.x * rd.y - sd.y * rd.x);
				float t1 = (edge2.start.x + sd.x * t2 - player.center().x) / rd.x;

				if(t1 <= 0 || t2 < 0 || t2 > 1.0f) continue;
				if(t1 >= min_t1) continue;

				min_t1 = t1;
				p.pos = player.center() + rd * t1;
				p.angle = atan2f(p.pos.y - player.center().y, p.pos.x - player.center().x);
				valid = true;
			}
			if(valid)// Add intersection point to visibility polygon perimeter
				points.push_back(p);
		}
	}
	//std::sort(points.begin(), points.end(), angleCompare);
	return points;
};
*/

std::vector<AnglePoint> getPolygon(World& world, Player& player) {
	std::vector<AnglePoint> points;

	float radius = tilesize * 40;

	for(auto edge : world.edges) {
		for(int i = 0; i < 2; i++) {
			float rdx = (i == 0 ? edge.start.x : edge.end.x) - player.location.x;
			float rdy = (i == 0 ? edge.start.y : edge.end.y) - player.location.y;

			float bangle = atan2f(rdy, rdx);
			float ang = 0;
			for(int j = 0; j < 3; j++) {
				if(j == 0) ang = bangle - 0.0001f;
				if(j == 1) ang = bangle;
				if(j == 2) ang = bangle + 0.0001f;

				rdx = radius * cosf(ang);
				rdy = radius * sinf(ang);

				float min_t1 = INFINITY;
				float min_px = 0, min_py = 0, min_ang = 0;
				bool bValid = false;

				for(auto edge2 : world.edges) {
					float sdx = edge2.end.x - edge2.start.x;
					float sdy = edge2.end.y - edge2.start.y;

					if(!fabs(sdx - rdx) > 0.0f || !fabs(sdy - rdy) > 0.0f) continue; //colleniar

					float t2 = (rdx * (edge2.end.y - player.location.y) + (rdy * (player.location.x - edge2.start.x))) / (sdx * rdy - sdy * rdx);
					float t1 = (edge2.start.x + sdx * t2 - player.location.x) / rdx;

					if(t1 <= 0 || t2 < 0 || t2 > 1.0f) continue;
					if(t1 >= min_t1) continue;

					min_t1 = t1;
					min_px = player.location.x + rdx * t1;
					min_py = player.location.y + rdy * t1;
					min_ang = atan2f(min_py - player.location.y, min_px - player.location.x);
					bValid = true;
				}
				if(bValid)// Add intersection point to visibility polygon perimeter
					points.push_back({min_ang, {min_px, min_py}});
			}
		}
	}

	std::sort(points.begin(), points.end(), angleCompare);
	return points;
}