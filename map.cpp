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
				edgecells[offset].edgeAt[i] = nullptr;
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
	calculateEdges(sf::Vector2f(x, y));
	update();
};
void Map::placeBlock(int block, sf::Vector2f loc) {
	placeBlock(block, (int)loc.x, (int)loc.y);
};

void Map::deleteEdge(Edge* edge) {
	auto res = std::find(edges.begin(), edges.end(), edge);
	if(res == edges.end()) {
		printf("Tried to delete an edge that isn't in a list (%f, %f) -> (%f, %f)!\n", (*edge).start.x, (*edge).start.y, (*edge).end.x, (*edge).end.y);
		return;
	}
	edges.erase(res);
}

void Map::cutEdge(sf::Vector2f blockpos, int edgeType) {
	int cur = blockpos.y * width + blockpos.x;
	//if edge doesn't exist, don't even try deleting it.
	if(!edgecells[cur].edgeExists[edgeType]) return;

	//left/right block for horizontal, up/down for vertical
	bool led = edgecells[cur - ((edgeType == Up || edgeType == Down) ? 1 : width)].edgeExists[edgeType];
	bool red = edgecells[cur + ((edgeType == Up || edgeType == Down) ? 1 : width)].edgeExists[edgeType];
	//if those edges don't exist, just delete the edge without any extra computation
	if(!led && !red)
		deleteEdge(edgecells[cur].edgeAt[edgeType]);
	else {
		sf::Vector2f blockloc = blockpos * (float)tilesize;
		sf::Vector2f bw = {tilesize, 0};
		sf::Vector2f bh = {0, tilesize};
		switch(edgeType) {
			case Up: {
				if(led && !red) edgecells[cur].edgeAt[Up]->end.x -= bw.x; //if only left edge exists, then current edge was the end of the edge, so roll back the end by block size
				else if(!led && red) edgecells[cur].edgeAt[Up]->start.x += bw.x; //sam, but the current edge was the start
				else { //if both sides exist, then we need to divide the edge into 2 edges.
					Edge* edge = new Edge();
					(*edge).start = blockloc + bw;
					(*edge).end = edgecells[cur].edgeAt[Up]->end;
					edgecells[cur].edgeAt[Up]->end = blockloc;
					edges.push_back(edge);
					for(int i = cur; i < cur + (((*edge).end.x - (*edge).start.x) / 16); i++)
						edgecells[i + 1].edgeAt[Up] = edge;
				}
			} break;
			case Right: {
				if(led && !red) edgecells[cur].edgeAt[Right]->end.y -= bh.y;
				else if(!led && red) edgecells[cur].edgeAt[Right]->start.y += bh.y;
				else {
					Edge* edge = new Edge();
					(*edge).start = blockloc + bw + bh;
					(*edge).end = edgecells[cur].edgeAt[Right]->end;
					edgecells[cur].edgeAt[Right]->end = blockloc + bw;
					edges.push_back(edge);
					for(int i = cur; i < cur + (((*edge).end.y - (*edge).start.y) * width / 16); i += width)
						edgecells[i + width].edgeAt[Right] = edge;
				}
			} break;
			case Down: {
				if(!led && red) edgecells[cur].edgeAt[Down]->end.x += bw.x;
				else if(led && !red) edgecells[cur].edgeAt[Down]->start.x -= bw.x;
				else {
					Edge* edge = new Edge();
					(*edge).start = blockloc + bh;
					(*edge).end = edgecells[cur].edgeAt[Down]->end;
					edgecells[cur].edgeAt[Down]->end = blockloc + bw + bh;
					edges.push_back(edge);
					for(int i = cur; i > cur - (((*edge).start.x - (*edge).end.x) / 16); i--)
						edgecells[i - 1].edgeAt[Down] = edge;
				}
			} break;
			case Left: {
				if(!led && red) edgecells[cur].edgeAt[Left]->end.y += bh.y;
				else if(led && !red) edgecells[cur].edgeAt[Left]->start.y -= bh.y;
				else {
					Edge* edge = new Edge();
					(*edge).start = blockloc;
					(*edge).end = edgecells[cur].edgeAt[Left]->end;
					edgecells[cur].edgeAt[Left]->end = blockloc + bh;
					edges.push_back(edge);
					for(int i = cur; i > cur - (((*edge).start.y - (*edge).end.y) * width / 16); i -= width)
						edgecells[i - width].edgeAt[Left] = edge;
				}
			} break;
		}
	}
	edgecells[cur].edgeAt[edgeType] = nullptr;
	edgecells[cur].edgeExists[edgeType] = false;
}
void Map::createEdge(sf::Vector2f blockpos, int edgeType) {

}

void Map::calculateEdges(sf::Vector2f blockpos) {
	int cur = blockpos.y * width + blockpos.x;
	if(cells[cur].type == 0) { //block got removed
		cutEdge(blockpos, Up);
		cutEdge(blockpos, Right);
		cutEdge(blockpos, Down);
		cutEdge(blockpos, Left);
	} else { //block got placed

	}
}

void Map::recalculateEdges() {
	edgecells = new CellEdges[width * height];
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int offset = y * width + x;
			for(int i = 0; i < 4; i++) {
				edgecells[offset].edgeAt[i] = nullptr;
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
					edgecells[left].edgeAt[Up]->end.x += tilesize;
					edgecells[cur].edgeAt[Up] = edgecells[left].edgeAt[Up];
					edgecells[cur].edgeExists[Up] = true;
				} else {
					Edge* edge = new Edge();
					(*edge).start = sf::Vector2f(x, y) * (float)tilesize;
					(*edge).end = sf::Vector2f((*edge).start.x + tilesize, (*edge).start.y);
					edgecells[cur].edgeAt[Up] = edge;
					edgecells[cur].edgeExists[Up] = true;
					edges.push_back(edge);
				}
			}
			if(x == width - 1 || cells[right].type == 0) {
				if(y != 0 && edgecells[up].edgeExists[Right]) {
					edgecells[up].edgeAt[Right]->end.y += tilesize;
					edgecells[cur].edgeAt[Right] = edgecells[up].edgeAt[Right];
					edgecells[cur].edgeExists[Right] = true;
				} else {
					Edge* edge = new Edge();
					(*edge).start = sf::Vector2f(x + 1, y) * (float)tilesize;
					(*edge).end = sf::Vector2f((*edge).start.x, (*edge).start.y + tilesize);
					edgecells[cur].edgeAt[Right] = edge;
					edgecells[cur].edgeExists[Right] = true;
					edges.push_back(edge);
				}
			}
			if(y == height - 1 || cells[down].type == 0) {
				if(x != 0 && edgecells[left].edgeExists[Down]) {
					edgecells[left].edgeAt[Down]->start.x += tilesize;
					edgecells[cur].edgeAt[Down] = edgecells[left].edgeAt[Down];
					edgecells[cur].edgeExists[Down] = true;
				} else {
					Edge* edge = new Edge();
					(*edge).end = sf::Vector2f(x, y + 1) * (float)tilesize;
					(*edge).start = sf::Vector2f((*edge).end.x + tilesize, (*edge).end.y);
					edgecells[cur].edgeAt[Down] = edge;
					edgecells[cur].edgeExists[Down] = true;
					edges.push_back(edge);
				}
			}
			if(x == 0 || cells[left].type == 0) {
				if(y != 0 && edgecells[up].edgeExists[Left]) {
					edgecells[up].edgeAt[Left]->start.y += tilesize;
					edgecells[cur].edgeAt[Left] = edgecells[up].edgeAt[Left];
					edgecells[cur].edgeExists[Left] = true;
				} else {
					Edge* edge = new Edge();
					(*edge).end = sf::Vector2f(x, y) * (float)tilesize;
					(*edge).start = sf::Vector2f((*edge).end.x, (*edge).end.y + tilesize);
					edgecells[cur].edgeAt[Left] = edge;
					edgecells[cur].edgeExists[Left] = true;
					edges.push_back(edge);
				}
			}
		}
	}

	edges.push_back(new Edge({sf::Vector2f(0, 0), sf::Vector2f(tilesize * width, 0)}));
	edges.push_back(new Edge({sf::Vector2f(tilesize * width, 0), sf::Vector2f(tilesize * width, tilesize * height)}));
	edges.push_back(new Edge({sf::Vector2f(tilesize * width, tilesize * height), sf::Vector2f(0, tilesize * height)}));
	edges.push_back(new Edge({sf::Vector2f(0, tilesize * height), sf::Vector2f(0, 0)}));
}

