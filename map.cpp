#include "map.h"

Map::Map(int w = 25, int h = 25) {
	width = w;
	height = h;
	grid = sf::VertexArray(sf::Lines, 2);
	edgesdraw = sf::VertexArray(sf::Lines, 2);

	auto gridcolor = sf::Color(255, 255, 255, 100);
	for(int x = 0; x <= width; x++) {
		grid.append(sf::Vertex(sf::Vector2f(x * tilesize, 0.f), gridcolor));
		grid.append(sf::Vertex(sf::Vector2f(x * tilesize, height * tilesize), gridcolor));
	};
	for(int y = 0; y <= height; y++) {
		grid.append(sf::Vertex(sf::Vector2f(0.f, y * tilesize), gridcolor));
		grid.append(sf::Vertex(sf::Vector2f(width * tilesize, y * tilesize), gridcolor));
	};

	cells = new Cell[width * height];
	edgecells = new CellEdges[width * height];
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int offset = y * width + x;
			for(int i = 0; i < 4; i++) {
				edgecells[offset].edgeId[i] = -1;
				edgecells[offset].edgeExists[i] = false;
			}
		}
	}
};

void Map::update() {

}

void Map::redraw() {

}

void Map::draw(sf::RenderWindow& window) {
	window.draw(grid);

	sf::RectangleShape tile;
	tile.setSize(sf::Vector2f(tilesize, tilesize));
	tile.setFillColor(sf::Color::Red);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			if(cells[y * width + x].type == 1) {
				tile.setPosition(x * tilesize, y * tilesize);
				window.draw(tile);
			}
		}
	}
}

bool Map::isInBounds(sf::Vector2f point) {
	return point.x >= 0 && point.x < width* tilesize&& point.y >= 0 && point.y < height* tilesize;
};

void Map::placeBlock(int block, int x, int y) {
	int loc = y * width + x;
	if(cells[loc].type == block) return;
	cells[loc].type = block;
	//calculateEdges(sf::Vector2f(x, y));
	update();
};
void Map::placeBlock(int block, sf::Vector2f loc) {
	placeBlock(block, (int)loc.x, (int)loc.y);
};

void Map::cutEdge(int edgeType, CellEdges cell, sf::Vector2f bpos, int index) {
	int id = cell.edgeId[edgeType];
	sf::Vector2f bw = {tilesize, 0};
	sf::Vector2f bh = {0, tilesize};
	switch(edgeType) {
		case Up: {
			if(edges[id].start == bpos) { //edge starts with this block
				edges[id].start += bw;
			} else if(edges[id].end == bpos + bw) { //edge ends with this block
				edges[id].end -= bw;
			} else { //block is in the center
				Edge nedge;
				nedge.start = bpos + bw;
				nedge.end = edges[id].end;

				int newid = edges.size();
				edges.push_back(nedge);
				edges[id].end = bpos;
				for(int i = index +1; i <= index + ((nedge.end.x-nedge.start.x)/16); i++) {
					edgecells[i].edgeId[Up] = newid;
				}
				return;
			}
			if(edges[id].start == edges[id].end) {
				edges.erase(edges.begin()+id);
			}
			return;
		}
	}
}

void Map::calculateEdges(sf::Vector2f blockloc) {
	int cur = blockloc.y * width + blockloc.x;
	if(cells[cur].type == 0) { //block got removed
		if(cells[Up].type == 0) {
			//assume top left and top right blocks are empty
			cutEdge(Up, edgecells[cur], blockloc * (float)tilesize, cur);
		}
	} else { //block got placed

	}
}


//don't use, need todo dynamically recalculate edges.
void Map::recalculateEdges() {
	edgecells = new CellEdges[width * height];
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int offset = y * width + x;
			for(int i = 0; i < 4; i++) {
				edgecells[offset].edgeId[i] = -1;
				edgecells[offset].edgeExists[i] = false;
			}
		}
	}
	edges.clear();

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int cur = y * width + x;

			if(cells[cur].type == 0) continue;

			int up = (y - 1) * width + x;
			int left = y * width + x - 1;
			int right = y * width + x + 1;
			int down = (y + 1) * width + x;

			if(y == 0 || cells[up].type == 0) {
				if(x != 0 && edgecells[left].edgeExists[Up]) {
					edges[edgecells[left].edgeId[Up]].end.x += tilesize;
					edgecells[cur].edgeId[Up] = edgecells[left].edgeId[Up];
					edgecells[cur].edgeExists[Up] = true;
				} else {
					Edge edge;
					edge.start = sf::Vector2f(x, y) * (float)tilesize;
					edge.end = sf::Vector2f(edge.start.x + tilesize, edge.start.y);

					int id = edges.size();
					edges.push_back(edge);

					edgecells[cur].edgeId[Up] = id;
					edgecells[cur].edgeExists[Up] = true;
				}
			}
			if(x == width - 1 || cells[right].type == 0) {
				if(y != 0 && edgecells[up].edgeExists[Right]) {
					edges[edgecells[up].edgeId[Right]].end.y += tilesize;
					edgecells[cur].edgeId[Right] = edgecells[up].edgeId[Right];
					edgecells[cur].edgeExists[Right] = true;
				} else {
					Edge edge;
					edge.start = sf::Vector2f(x + 1, y) * (float)tilesize;
					edge.end = sf::Vector2f(edge.start.x, edge.start.y + tilesize);

					int id = edges.size();
					edges.push_back(edge);

					edgecells[cur].edgeId[Right] = id;
					edgecells[cur].edgeExists[Right] = true;
				}
			}
			if(y == height - 1 || cells[down].type == 0) {
				if(x != 0 && edgecells[left].edgeExists[Down]) {
					edges[edgecells[left].edgeId[Down]].start.x += tilesize;
					edgecells[cur].edgeId[Down] = edgecells[left].edgeId[Down];
					edgecells[cur].edgeExists[Down] = true;
				} else {
					Edge edge;
					edge.end = sf::Vector2f(x, y + 1) * (float)tilesize;
					edge.start = sf::Vector2f(edge.end.x + tilesize, edge.end.y);

					int id = edges.size();
					edges.push_back(edge);

					edgecells[cur].edgeId[Down] = id;
					edgecells[cur].edgeExists[Down] = true;
				}
			}
			if(x == 0 || cells[left].type == 0) {
				if(y != 0 && edgecells[up].edgeExists[Left]) {
					edges[edgecells[up].edgeId[Left]].start.y += tilesize;
					edgecells[cur].edgeId[Left] = edgecells[up].edgeId[Left];
					edgecells[cur].edgeExists[Left] = true;
				} else {
					Edge edge;
					edge.end = sf::Vector2f(x, y) * (float)tilesize;
					edge.start = sf::Vector2f(edge.end.x, edge.end.y + tilesize);

					int id = edges.size();
					edges.push_back(edge);

					edgecells[cur].edgeId[Left] = id;
					edgecells[cur].edgeExists[Left] = true;
				}
			}
		}
	}
	Edge edge;
	edge.start = sf::Vector2f(0, 0);
	edge.end = sf::Vector2f(tilesize * width, 0);
	edges.push_back(edge);
	edge.start = sf::Vector2f(tilesize * width, 0);
	edge.end = sf::Vector2f(tilesize * width, tilesize * height);
	edges.push_back(edge);
	edge.start = sf::Vector2f(tilesize * width, tilesize * height);
	edge.end = sf::Vector2f(0, tilesize * height);
	edges.push_back(edge);
	edge.start = sf::Vector2f(0, tilesize * height);
	edge.end = sf::Vector2f(0, 0);
	edges.push_back(edge);
}

