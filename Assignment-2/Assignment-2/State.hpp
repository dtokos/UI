#ifndef State_hpp
#define State_hpp

#include <vector>
#include <string>

using namespace std;

class State {
public:
	struct Size {
		const int width, height;
		
		bool operator ==(const Size &b) const {return width == b.width && height == b.height;}
		bool operator !=(const Size &b) const {return width != b.width || height != b.height;}
		
		string toString() const {return "(" + to_string(width) + ", " + to_string(height) + ")";}
	};
	
	State(Size size, vector<int> tiles) : size(size), tiles(tiles) {}
	
	bool operator ==(const State &b) const {
		return size == b.size && tiles == b.tiles;
	}
	
	const Size size;
	const vector<int> tiles;
};

#endif
