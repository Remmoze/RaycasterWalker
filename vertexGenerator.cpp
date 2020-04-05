#include "vertexGenerator.h"
#include "world.h"


/*
TODO: convert a selected region, not the whole world.
*/
namespace {
	vCell* populateCells(World& world) {
		vCell* cells = new vCell[world.width * world.height];
		for(int y = 0; y < world.height; y++) {
			for(int x = 0; x < world.width; x++) {
				int offset = y * world.width + x;
				for(int i = 0; i < 4; i++) {
					cells[offset].edgeId[i] = 0;
					cells[offset].edgeExists[i] = false;
				}
			}
		}
		return cells;
	}
	enum {
		Up, Left, Right, Down
	};
}
void convert(World& world) {
	convert(world, sf::Vector2f(0, 0), sf::Vector2f(world.width, world.height));
}

void convert(World& world, sf::Vector2f start, sf::Vector2f size) {
	vCell* cells = populateCells(world);
	world.edges.clear();

	for(int y = 0; y < world.height; y++) {
		for(int x = 0; x < world.width; x++) {
			int cur = y * world.width + x;

			if(world.cells[cur].type == 0) continue;

			int up = (y - 1) * world.width + x;
			int left = y * world.width + x - 1;
			int right = y * world.width + x + 1;
			int down = (y + 1) * world.width + x;

			cells[up].edgeExists[1];

			if(y == 0 || world.cells[up].type == 0) {
				if(x != 0 && cells[left].edgeExists[Up]) {
					world.edges[cells[left].edgeId[Up]].end.x += tilesize;
					cells[cur].edgeId[Up] = cells[left].edgeId[Up];
					cells[cur].edgeExists[Up] = true;
				} else {
					Edge edge;
					edge.start = sf::Vector2f(x, y) * (float)tilesize;
					edge.end = sf::Vector2f(edge.start.x + tilesize, edge.start.y);

					int id = world.edges.size();
					world.edges.push_back(edge);

					cells[cur].edgeId[Up] = id;
					cells[cur].edgeExists[Up] = true;
				}
			}
			if(x == world.width-1 || world.cells[right].type == 0) {
				if(y != 0 && cells[up].edgeExists[Right]) {
					world.edges[cells[up].edgeId[Right]].end.y += tilesize;
					cells[cur].edgeId[Right] = cells[up].edgeId[Right];
					cells[cur].edgeExists[Right] = true;
				} else {
					Edge edge;
					edge.start = sf::Vector2f(x+1, y) * (float)tilesize;
					edge.end = sf::Vector2f(edge.start.x, edge.start.y + tilesize);

					int id = world.edges.size();
					world.edges.push_back(edge);

					cells[cur].edgeId[Right] = id;
					cells[cur].edgeExists[Right] = true;
				}
			}
			if(y == world.height - 1 || world.cells[down].type == 0) {
				if(x != 0 && cells[left].edgeExists[Down]) {
					world.edges[cells[left].edgeId[Down]].start.x += tilesize;
					cells[cur].edgeId[Down] = cells[left].edgeId[Down];
					cells[cur].edgeExists[Down] = true;
				} else {
					Edge edge;
					edge.end = sf::Vector2f(x, y+1) * (float)tilesize;
					edge.start = sf::Vector2f(edge.end.x + tilesize, edge.end.y);

					int id = world.edges.size();
					world.edges.push_back(edge);

					cells[cur].edgeId[Down] = id;
					cells[cur].edgeExists[Down] = true;
				}
			}
			if(x == 0 || world.cells[left].type == 0) {
				if(y != 0 && cells[up].edgeExists[Left]) {
					world.edges[cells[up].edgeId[Left]].start.y += tilesize;
					cells[cur].edgeId[Left] = cells[up].edgeId[Left];
					cells[cur].edgeExists[Left] = true;
				} else {
					Edge edge;
					edge.end = sf::Vector2f(x, y) * (float)tilesize;
					edge.start = sf::Vector2f(edge.end.x, edge.end.y + tilesize);

					int id = world.edges.size();
					world.edges.push_back(edge);

					cells[cur].edgeId[Left] = id;
					cells[cur].edgeExists[Left] = true;
				}
			}
		}
	}
	world.edges;
}

