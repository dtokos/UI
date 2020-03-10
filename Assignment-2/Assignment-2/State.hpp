#ifndef State_hpp
#define State_hpp

#include <vector>
#include <string>
#include <sstream>
#include <optional>

using namespace std;

class State {
public:
	struct Size {
		int width, height;
		
		bool operator ==(const Size &b) const {return width == b.width && height == b.height;}
		bool operator !=(const Size &b) const {return width != b.width || height != b.height;}
		
		string toString() const {return "(" + to_string(width) + ", " + to_string(height) + ")";}
	};
	
	struct Position {
		int x, y;
		
		bool operator ==(const Position &b) const {return x == b.x && y == b.y;}
		bool operator !=(const Position &b) const {return x != b.x || y != b.y;}
		
		int toIndex(Size size) const {return y * size.width + x;}
		string toString() const {return "(" + to_string(x) + ", " + to_string(y) + ")";}
	};
	
	struct Score {int f, g, h;};
	enum Direction {Top, Left, Right, Bottom};
	
	State(Size size, vector<int> tiles, Position emptyPos) :
		size(size),
		tiles(tiles),
		emptyPosition(emptyPos),
		parent(NULL),
		parentDirection(Top) {}
	State(Size size, vector<int> tiles, Score score, Position emptyPos, const State *parent, Direction parentDirection) :
		size(size),
		tiles(tiles),
		score(score),
		emptyPosition(emptyPos),
		parent(parent),
		parentDirection(parentDirection) {}
	State(Size size, vector<int> tiles) :
		size(size),
		tiles(tiles),
		emptyPosition(findEmptyCoordinates()),
		parent(NULL),
		parentDirection(Top) {}
	
	Size size;
	vector<int> tiles;
	Score score;
	Position emptyPosition;
	
	const State *parent;
	Direction parentDirection;
	
	bool operator ==(const State &b) const {
		return size == b.size && emptyPosition == b.emptyPosition && tiles == b.tiles;
	}
	
	optional<State> getNeighbour(Direction direction) const {
		Position nextPos = neighbourPosition(direction);
		
		if (isNeighbourOutOfBounds(nextPos))
			return nullopt;
		
		vector<int> nextTiles = tiles;
		swap(nextTiles[emptyPosition.toIndex(size)], nextTiles[nextPos.toIndex(size)]);
		
		return State{size, nextTiles, {score.f, score.g + 1, score.h}, nextPos, this, direction};
	}
	
private:
	Position findEmptyCoordinates() {
		for (int i = 0; i < tiles.size(); i++)
			if (tiles[i] == -1)
				return {i % size.width, i / size.height};
		
		throw "Empty coordinates could not be found";
	}
		
	Position neighbourPosition(Direction direction) const {
		switch (direction) {
			case Top: return {emptyPosition.x, emptyPosition.y - 1};
			case Left: return {emptyPosition.x - 1, emptyPosition.y};
			case Right: return {emptyPosition.x + 1, emptyPosition.y};
			case Bottom: return {emptyPosition.x, emptyPosition.y + 1};
		}
	}
	
	bool isNeighbourOutOfBounds(Position pos) const {
		return pos.x < 0 || pos.x >= size.width || pos.y < 0 || pos.y >= size.height;
	}
};

#endif
