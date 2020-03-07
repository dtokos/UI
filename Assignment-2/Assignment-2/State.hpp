#ifndef State_hpp
#define State_hpp

#include <vector>

using namespace std;

class State {
public:
	struct Size {
		const int width, height;
		
		bool operator ==(const Size &b) const {
			return width == b.width && height == b.height;
		}
	};
	
	State(Size size, vector<int> tiles) : size(size), tiles(tiles) {}
	
	bool operator ==(const State &b) const {
		return size == b.size && tiles == b.tiles;
	}
	
	const Size size;
	const vector<int> tiles;
};

#endif
