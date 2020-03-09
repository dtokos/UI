#ifndef Heuristics_hpp
#define Heuristics_hpp

#include <map>
#include <numeric>
#include <functional>
#include "./State.hpp"

using namespace std;

class Heuristics {
public:
	virtual void setFinal(State *finalState) = 0;
	virtual int evaluate(State *curentState) = 0;
};

class BaseHeuristics : public Heuristics {
public:
	void setFinal(State *fS) {finalState = fS;}

protected:
	State *finalState;
};

class SpotHeuristics : public BaseHeuristics {
public:
	int evaluate(State *curentState) {
		int numOfDiffTiles = 0;
		
		for (int i = 0; i < curentState->tiles.size(); i++)
			if (curentState->tiles[i] != finalState->tiles[i])
				numOfDiffTiles++;
		
		return numOfDiffTiles;
	}
};

class DistanceHeuristics : public BaseHeuristics {
private:
	struct Point {
		int x, y;
		
		int distance(Point &b) {
			return abs(x - b.x) + abs(y - b.y);
		}
	};
	
public:
	int evaluate(State *curentState) {
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
	using TransformFnc = function<int(int, int)>;
	GroupHeuristics(vector<Heuristics *> heuristics, TransformFnc transformFnc) : heuristics(heuristics), transformFnc(transformFnc) {}
	
	void setFinal(State *finalState) {
		for (auto h : heuristics)
			h->setFinal(finalState);
	}
	
	int evaluate(State *curentState) {
		return accumulate(heuristics.begin(), heuristics.end(), 0, [this, curentState](int acc, Heuristics *h) {
			return transformFnc(acc, h->evaluate(curentState));
		});
	}
	
private:
	vector<Heuristics *> heuristics;
	TransformFnc transformFnc;
};

class SumHeuristics : public GroupHeuristics {
public:
	SumHeuristics(vector<Heuristics *> heuristics) :
		GroupHeuristics(heuristics, [](int acc, int result) { return acc + result; }) {}
};


#endif
