#ifndef Map_hpp
#define Map_hpp

#include <vector>
#include <unordered_set>
#include <set>

using namespace std;

class Map {
public:
	struct Vector {
		int x, y;
		
		bool operator==(const Vector& b) const {return x == b.x && y == b.y;}
		bool operator!=(const Vector& b) const {return x != b.x || y != b.y;}
		bool operator<(const Vector& b) const {
			if (x == b.x) return y < b.y;
			else return x < b.x;
		}
	};
	
	Map(const Vector &si, const Vector &st, const vector<Vector> &t) :
		size(si),
		start(st),
		treasures(t.begin(), t.end()) {}
	
	bool isOnTreasure(Vector position) {
		return treasures.find(position) != treasures.end();
	}
	
	size_t treasureCount() {
		return treasures.size();
	}
	
	const Vector size;
	const Vector start;

private:
	set<Vector> treasures;
};

#endif
