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
		
		int toIndex(Size size) {
			return y * size.width + x;
		}
	};
	
	struct Score {
		int f, g, h;
	};
	
	enum Direction {Top, Left, Right, Bottom};
	
	State(Size size, vector<int> tiles, Position emptyPos) : size(size), tiles(tiles), emptyPosition(emptyPos) {}
	State(Size size, vector<int> tiles) : size(size), tiles(tiles), emptyPosition(findEmptyCoordinates()) {}
	State(const State &old) : size(old.size), tiles(old.tiles), score(old.score), emptyPosition(old.emptyPosition) {}
	
	Size size;
	vector<int> tiles;
	Score score;
	Position emptyPosition;
	
	bool operator ==(const State &b) const {
		return size == b.size && emptyPosition == b.emptyPosition && tiles == b.tiles;
	}
	
	string getID() {
		ostringstream result;
		for (auto tile : tiles)
			result << tile << ",";
		
		return result.str();
	}
	
	optional<State> getNeighbour(Direction direction) {
		Position nextPos = neighbourPosition(direction);
		
		if (isNeighbourOutOfBounds(nextPos))
			return nullopt;
		
		vector<int> nextTiles = tiles;
		swap(nextTiles[emptyPosition.toIndex(size)], nextTiles[nextPos.toIndex(size)]);
		
		return State{size, nextTiles, nextPos};
	}
	
private:
	Position findEmptyCoordinates() {
		for (int i = 0; i < tiles.size(); i++)
			if (tiles[i] == -1)
				return {i % size.width, i / size.height};
		
		throw "Empty coordinates could not be found";
	}
		
	Position neighbourPosition(Direction direction) {
		switch (direction) {
			case Top: return {emptyPosition.x, emptyPosition.y - 1};
			case Left: return {emptyPosition.x - 1, emptyPosition.y};
			case Right: return {emptyPosition.x + 1, emptyPosition.y};
			case Bottom: return {emptyPosition.x, emptyPosition.y + 1};
		}
	}
	
	bool isNeighbourOutOfBounds(Position pos) {
		return pos.x < 0 || pos.x >= size.width || pos.y < 0 || pos.y >= size.height;
	}
};

#endif
