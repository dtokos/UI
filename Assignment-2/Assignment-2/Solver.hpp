#ifndef Solver_hpp
#define Solver_hpp

#include <map>
#include <set>
#include <queue>
#include <string>
#include <unordered_set>
#include "./Heuristics.hpp"

using namespace std;
using Direction = State::Direction;

class Solver {
public:
	struct SolvingException : public exception {
		string message;
		SolvingException(string message) : message(message) {};
		const char* what() const throw() { return message.c_str(); }
	};
	
	Solver(Heuristics *h) : heuristics(h) {}
	
	// Return type will be different
	void solve(State &start, State &finish) {
		if (start.size != finish.size)
			throw SolvingException("State sizes doesnt match: " + start.size.toString() + " and " + finish.size.toString());
		
		clear();
		findSolution(start, finish);
	}
	
private:
	struct HeuristicCompare {
		bool operator()(const State &lhs, const State &rhs) const {
			return lhs.score.f < rhs.score.f;
		}
	};
	
	struct StateHasher {
		size_t operator()(const State &s) const {
			size_t hash = s.tiles.size();
			for (auto& tile : s.tiles)
				hash ^= tile + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			
			return hash;
		}
	};
	
	Heuristics *heuristics;
	priority_queue<State, vector<State>, HeuristicCompare> open;
	unordered_set<State, StateHasher> closed;
	
	void clear() {
		open = priority_queue<State, vector<State>, HeuristicCompare>();
		closed.clear();
	}
	
	void findSolution(State &start, State &finish) {
		start.score.g = 0;
		heuristics->setFinal(&finish);
		pushOpen(start);
		
		while (!open.empty()) {
			State state = popOpen();
			
			if (state == finish)
				return; // Found path, traceback
			
			closeState(state);
			appendNeighbours(state);
		}
		
		throw SolvingException("Solution doesnt exist");
	}
	
	void pushOpen(State &state) {
		calculateScore(state);
		open.push(state);
	}
	
	State popOpen() {
		State s = open.top();
		open.pop();
		return s;
	}
	
	void closeState(State &state) {
		closed.insert(state);
	}
	
	void appendNeighbours(State &state) {
		constexpr Direction allDirections[] = {Direction::Top, Direction::Left, Direction::Right, Direction::Bottom};
		
		for (auto direction : allDirections) {
			optional<State> neighbour = state.getNeighbour(direction);
			
			if (neighbour != nullopt && closed.find(*neighbour) == closed.end()) {
				neighbour->score.g++;
				pushOpen(*neighbour);
			}
		}
	}
	
	void calculateScore(State &state) {
		state.score.h = heuristics->evaluate(&state);
		state.score.f = state.score.g + state.score.h;
	}
};

#endif
