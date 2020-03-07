#ifndef Heuristics_hpp
#define Heuristics_hpp

#include <map>
#include <numeric>
#include <functional>
#include "./State.hpp"

using namespace std;

class Heuristics {
public:
	virtual int evaluate(State *curentState, State *finalState) = 0;
};

class SpotHeuristics : public Heuristics {
public:
	int evaluate(State *curentState, State *finalState) {
		int numOfDiffTiles = 0;
		
		for (int i = 0; i < curentState->tiles.size(); i++)
			if (curentState->tiles[i] != finalState->tiles[i])
				numOfDiffTiles++;
		
		return numOfDiffTiles;
	}
};

class DistanceHeuristics : public Heuristics {
private:
	struct Point {
		int x, y;
		
		int distance(Point &b) {
			return abs(x - b.x) + abs(y - b.y);
		}
	};
	
public:
	int evaluate(State *curentState, State *finalState) {
		int distance = 0;
		Point point;
		map<int, Point> lookup;
		
		for (int i = 0; i < finalState->tiles.size(); i++)
			lookup[finalState->tiles[i]] = {i % finalState->size.width, i / finalState->size.width};
		
		for (int i = 0; i < curentState->tiles.size(); i++) {
			point.x = i % curentState->size.width;
			point.y = i / curentState->size.width;
			distance += point.distance(lookup[curentState->tiles[i]]);
		}
		
		return distance;
	}
};

class GroupHeuristics : public Heuristics {
public:
	using TransformFnc = function<int(State *, State *, int, Heuristics *)>;
	GroupHeuristics(vector<Heuristics *> heuristics, TransformFnc transformFnc) : heuristics(heuristics), transformFnc(transformFnc) {}
	
	int evaluate(State *curentState, State *finalState) {
		return accumulate(heuristics.begin(), heuristics.end(), 0, [this, curentState, finalState](int acc, Heuristics *h) {
			return transformFnc(curentState, finalState, acc, h);
		});
	}
	
private:
	vector<Heuristics *> heuristics;
	TransformFnc transformFnc;
	
};

class SumHeuristics : public GroupHeuristics {
public:
	SumHeuristics(vector<Heuristics *> heuristics) :
		GroupHeuristics(heuristics, [](State *c, State *f, int acc, Heuristics *h) { return acc + h->evaluate(c, f); }) {}
};


#endif
