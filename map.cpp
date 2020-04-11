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

	//world edges
	edges.push_back(new Edge({sf::Vector2f(0, 0), sf::Vector2f(tilesize * width, 0)}));
	edges.push_back(new Edge({sf::Vector2f(tilesize * width, 0), sf::Vector2f(tilesize * width, tilesize * height)}));
	edges.push_back(new Edge({sf::Vector2f(tilesize * width, tilesize * height), sf::Vector2f(0, tilesize * height)}));
	edges.push_back(new Edge({sf::Vector2f(0, tilesize * height), sf::Vector2f(0, 0)}));
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

bool Map::isLocInBounds(sf::Vector2f loc) {
	return loc.x >= 0 && loc.x < width* tilesize&& loc.y >= 0 && loc.y < height* tilesize;
};
bool Map::isPosInBounds(sf::Vector2f pos) {
	return pos.x >= 0 && pos.x < width&& pos.y >= 0 && pos.y < height;
};

void Map::placeBlock(int block, int x, int y) {
	int loc = y * width + x;
	if(cells[loc].type == block) return;
	cells[loc].type = block;
	calculateEdges(sf::Vector2i(x, y));
	update();
};
void Map::placeBlock(int block, sf::Vector2f loc) {
	placeBlock(block, (int)loc.x, (int)loc.y);
};

void Map::deleteEdge(Edge* edge) {
	if(edge == nullptr) {
		printf("Tried to delete an edge that doesn't exist!\n");
		return;
	}
	auto res = std::find(edges.begin(), edges.end(), edge);
	if(res == edges.end()) {
		printf("Tried to delete an edge that isn't in a list (%f, %f) -> (%f, %f)!\n", edge->start.x, edge->start.y, edge->end.x, edge->end.y);
		return;
	}
	edges.erase(res);
}

void Map::createEdge(sf::Vector2f blockloc, int cur, int edgeType) {
	sf::Vector2f bw = {tilesize, 0};
	sf::Vector2f bh = {0, tilesize};

	Edge* edge = new Edge();
	switch(edgeType) {
		case Up: {
			edge->start = blockloc;
			edge->end = blockloc + bw;
		} break;
		case Right: {
			edge->start = blockloc + bw;
			edge->end = blockloc + bw + bh;
		} break;
		case Down: {
			edge->start = blockloc + bw + bh;
			edge->end = blockloc + bh;
		} break;
		case Left: {
			edge->start = blockloc + bh;
			edge->end = blockloc;
		} break;
	}

	edges.push_back(edge);
	edgecells[cur].edgeAt[edgeType] = edge;
	edgecells[cur].edgeExists[edgeType] = true;
}

void Map::cutEdge(sf::Vector2f blockpos, int edgeType) {
	if(!isPosInBounds(blockpos)) return;
	int cur = blockpos.y * width + blockpos.x;
	//if edge doesn't exist, don't even try deleting it.
	if(!edgecells[cur].edgeExists[edgeType]) return;

	bool hor = edgeType == Up || edgeType == Down;
	bool ver = !hor;

	//left/right block for horizontal, up/down for vertical
	bool led, red;
	if(blockpos.x <= 0 && hor)
		led = false;
	else if(blockpos.y <= 0 && ver)
		led = false;
	else led = edgecells[cur - (hor ? 1 : width)].edgeExists[edgeType];

	if(blockpos.x >= width - 1 && hor)
		red = false;
	else if(blockpos.y >= height - 1 && ver)
		red = false;
	else red = edgecells[cur + (hor ? 1 : width)].edgeExists[edgeType];

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
				else if(!led && red) edgecells[cur].edgeAt[Up]->start.x += bw.x; //same, but the current edge was the start
				else { //if both sides exist, then we need to divide the edge into 2 edges.
					Edge* edge = new Edge();
					edge->start = blockloc + bw;
					edge->end = edgecells[cur].edgeAt[Up]->end;
					edgecells[cur].edgeAt[Up]->end = blockloc;
					edges.push_back(edge);
					for(int i = 0; i < (edge->end.x - edge->start.x) / tilesize; i++)
						edgecells[cur + i + 1].edgeAt[Up] = edge;
				}
			} break;
			case Right: {
				if(led && !red) edgecells[cur].edgeAt[Right]->end.y -= bh.y;
				else if(!led && red) edgecells[cur].edgeAt[Right]->start.y += bh.y;
				else {
					Edge* edge = new Edge();
					edge->start = blockloc + bw + bh;
					edge->end = edgecells[cur].edgeAt[Right]->end;
					edgecells[cur].edgeAt[Right]->end = blockloc + bw;
					edges.push_back(edge);
					for(int i = 0; i < (edge->end.y - edge->start.y) * width / tilesize; i += width)
						edgecells[cur + i + width].edgeAt[Right] = edge;
				}
			} break;
			case Down: {
				if(led && !red) edgecells[cur].edgeAt[Down]->start.x -= bw.x;
				else if(!led && red) edgecells[cur].edgeAt[Down]->end.x += bw.x;
				else {
					Edge* edge = new Edge();
					edge->start = blockloc + bh;
					edge->end = edgecells[cur].edgeAt[Down]->end;
					edgecells[cur].edgeAt[Down]->end = blockloc + bw + bh;
					edges.push_back(edge);
					for(int i = 0; i < (edge->start.x - edge->end.x) / tilesize; i++)
						edgecells[cur - i - 1].edgeAt[Down] = edge;
				}
			} break;
			case Left: {
				if(led && !red) edgecells[cur].edgeAt[Left]->start.y -= bh.y;
				else if(!led && red) edgecells[cur].edgeAt[Left]->end.y += bh.y;
				else {
					Edge* edge = new Edge();
					edge->start = blockloc;
					edge->end = edgecells[cur].edgeAt[Left]->end;
					edgecells[cur].edgeAt[Left]->end = blockloc + bh;
					edges.push_back(edge);
					for(int i = 0; i < (edge->start.y - edge->end.y) * width / tilesize; i += width)
						edgecells[cur - i - width].edgeAt[Left] = edge;
				}
			} break;
		}
	}
	edgecells[cur].edgeAt[edgeType] = nullptr;
	edgecells[cur].edgeExists[edgeType] = false;
}
void Map::wireEdge(sf::Vector2f blockpos, int edgeType) {
	if(!isPosInBounds(blockpos)) return;
	int cur = blockpos.y * width + blockpos.x;
	// air doesn't need edges duh.
	if(cells[cur].type == 0) return;
	if(edgeType == Up && cells[cur - width].type != 0) return;
	if(edgeType == Right && cells[cur + 1].type != 0) return;
	if(edgeType == Down && cells[cur + width].type != 0) return;
	if(edgeType == Left && cells[cur - 1].type != 0) return;

	bool hor = edgeType == Up || edgeType == Down;
	bool ver = !hor;

	//left/right block for horizontal, up/down for vertical
	bool led, red;
	if(blockpos.x <= 0 && hor)
		led = false;
	else if(blockpos.y <= 0 && ver)
		led = false;
	else led = edgecells[cur - (hor ? 1 : width)].edgeExists[edgeType];

	if(blockpos.x >= width - 1 && hor)
		red = false;
	else if(blockpos.y >= height - 1 && ver)
		red = false;
	else red = edgecells[cur + (hor ? 1 : width)].edgeExists[edgeType];

	if(!led && !red) // no edges are present, we need to create a new one.
		createEdge(blockpos * (float)tilesize, cur, edgeType);
	else {
		sf::Vector2f blockloc = blockpos * (float)tilesize;
		sf::Vector2f bw = {tilesize, 0};
		sf::Vector2f bh = {0, tilesize};
		switch(edgeType) {
			case Up: {
				if(led && !red) { //if only left edge exists, then continue the edge to include current block
					edgecells[cur - 1].edgeAt[Up]->end.x += bw.x;
					edgecells[cur].edgeAt[Up] = edgecells[cur - 1].edgeAt[Up];
				} else if(!led && red) { //same, but include edge as a start
					edgecells[cur + 1].edgeAt[Up]->start.x -= bw.x;
					edgecells[cur].edgeAt[Up] = edgecells[cur + 1].edgeAt[Up];
				} else { //if both sides exist, then we need to wire the edges together
					edgecells[cur - 1].edgeAt[Up]->end.x = edgecells[cur + 1].edgeAt[Up]->end.x; // take left edge and continue it to the end of the right edge.
					deleteEdge(edgecells[cur + 1].edgeAt[Up]); //delete the right edge
					for(int i = 0; i < (edgecells[cur - 1].edgeAt[Up]->end.x - blockloc.x) / tilesize + 1; i++)
						edgecells[cur + i].edgeAt[Up] = edgecells[cur - 1].edgeAt[Up]; //point all blocks of right edge to the left edge instead.
				}
			} break;
			case Right: {
				if(led && !red) {
					edgecells[cur - width].edgeAt[Right]->end.y += bh.y;
					edgecells[cur].edgeAt[Right] = edgecells[cur - width].edgeAt[Right];
				} else if(!led && red) {
					edgecells[cur + width].edgeAt[Right]->start.y -= bh.y;
					edgecells[cur].edgeAt[Right] = edgecells[cur + width].edgeAt[Right];
				} else {
					edgecells[cur - width].edgeAt[Right]->end.y = edgecells[cur + width].edgeAt[Right]->end.y;
					deleteEdge(edgecells[cur + width].edgeAt[Right]);
					for(int i = 0; i < (edgecells[cur - width].edgeAt[Right]->end.y - blockloc.y) / tilesize * width + width; i += width)
						edgecells[cur + i].edgeAt[Right] = edgecells[cur - width].edgeAt[Right];
				}
			} break;
			case Down: {
				if(led && !red) {
					edgecells[cur - 1].edgeAt[Down]->start.x += bw.x;
					edgecells[cur].edgeAt[Down] = edgecells[cur - 1].edgeAt[Down];
				} else if(!led && red) {
					edgecells[cur + 1].edgeAt[Down]->end.x -= bw.x;
					edgecells[cur].edgeAt[Down] = edgecells[cur + 1].edgeAt[Down];
				} else {
					edgecells[cur + 1].edgeAt[Down]->end.x = edgecells[cur - 1].edgeAt[Down]->end.x;
					deleteEdge(edgecells[cur - 1].edgeAt[Down]);
					for(int i = 0; i < (blockloc.x - edgecells[cur + 1].edgeAt[Down]->end.x) / tilesize + 1; i++)
						edgecells[cur - i].edgeAt[Down] = edgecells[cur + 1].edgeAt[Down];
				}
			} break;
			case Left: {
				if(led && !red) {
					edgecells[cur - width].edgeAt[Left]->start.y += bh.y;
					edgecells[cur].edgeAt[Left] = edgecells[cur - width].edgeAt[Left];
				} else if(!led && red) {
					edgecells[cur + width].edgeAt[Left]->end.y -= bh.y;
					edgecells[cur].edgeAt[Left] = edgecells[cur + width].edgeAt[Left];
				} else {
					edgecells[cur + width].edgeAt[Left]->end.y = edgecells[cur - width].edgeAt[Left]->end.y;
					deleteEdge(edgecells[cur - width].edgeAt[Left]);
					for(int i = 0; i < (blockloc.y - edgecells[cur + width].edgeAt[Left]->end.y) / tilesize * width + width; i += width)
						edgecells[cur - i].edgeAt[Left] = edgecells[cur + width].edgeAt[Left];
				}
			} break;
		}
		edgecells[cur].edgeExists[edgeType] = true;
	}
}

void Map::calculateEdges(sf::Vector2i blockpos) {
	int cur = blockpos.y * width + blockpos.x;
	auto blockposf = sf::Vector2f(blockpos);
	if(cells[cur].type == 0) { //block got removed
		cutEdge(blockposf, Up);
		wireEdge(blockposf + sf::Vector2f(0, -1), Down);

		cutEdge(blockposf, Right);
		wireEdge(blockposf + sf::Vector2f(1, 0), Left);

		cutEdge(blockposf, Down);
		wireEdge(blockposf + sf::Vector2f(0, 1), Up);

		cutEdge(blockposf, Left);
		wireEdge(blockposf + sf::Vector2f(-1, 0), Right);
	} else { //block got placed
		//doesn't work
		wireEdge(blockposf, Down);
		cutEdge(blockposf + sf::Vector2f(0, 1), Up);

		wireEdge(blockposf, Left);
		cutEdge(blockposf + sf::Vector2f(-1, 0), Right);

		wireEdge(blockposf, Up);
		cutEdge(blockposf + sf::Vector2f(0, -1), Down);

		wireEdge(blockposf, Right);
		cutEdge(blockposf + sf::Vector2f(1, 0), Left);
	}
}


//deprecated
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
					edge->start = sf::Vector2f(x, y) * (float)tilesize;
					edge->end = sf::Vector2f(edge->start.x + tilesize, edge->start.y);
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
					edge->start = sf::Vector2f(x + 1, y) * (float)tilesize;
					edge->end = sf::Vector2f(edge->start.x, edge->start.y + tilesize);
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
					edge->end = sf::Vector2f(x, y + 1) * (float)tilesize;
					edge->start = sf::Vector2f(edge->end.x + tilesize, edge->end.y);
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
					edge->end = sf::Vector2f(x, y) * (float)tilesize;
					edge->start = sf::Vector2f(edge->end.x, edge->end.y + tilesize);
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

