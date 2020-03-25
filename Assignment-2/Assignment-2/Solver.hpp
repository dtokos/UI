#ifndef Solver_hpp
#define Solver_hpp

#include <map>
#include <set>
#include <queue>
#include <string>
#include <unordered_set>
#include "./Heuristics.hpp"
#include "./MinHeap.hpp"

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
	
	vector<State> solve(State &start, State &finish) {
		if (start.size != finish.size)
			throw SolvingException("State sizes doesnt match: " + start.size.toString() + " and " + finish.size.toString());
		
		clear();
		return findSolution(start, finish);
	}
	
private:
	struct StateHasher {
		size_t operator()(const State &s) const {
			size_t hash = s.tiles.size();
			for (const auto& tile : s.tiles)
				hash ^= tile + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			
			return hash;
		}
	};
	
	Heuristics *heuristics;
	unordered_set<State, StateHasher> states;
	unordered_set<const State *> closed;
	MinHeap open;
	
	void clear() {
		open.clear();
		states.clear();
		closed.clear();
	}
	
	vector<State> findSolution(State &start, State &finish) {
		start.score.g = 0;
		heuristics->setFinal(&finish);
		pushOrUpdateOpenAndCloseDuplicates(start);
		
		while (!open.isEmpty()) {
			const State *state = popOpen();
			
			// Try comparing h score
			if (*state == finish)
				return traceback(state);
			
			close(state);
			appendNeighbours(state);
		}
		
		throw SolvingException("Solution doesnt exist");
	}
	
	void close(const State *s) {
		closed.insert(s);
	}
	
	void pushOrUpdateOpenAndCloseDuplicates(State &state) {
		calculateScore(state);
		const auto result = states.insert(state);
		const State *s = &(*result.first);
		
		if (result.second)
			open.push(s);
		else if (state.score.g < s->score.g && closed.find(s) == closed.end()) {
			s->score = state.score;
			s->parent = state.parent;
			s->parentDirection = state.parentDirection;
			open.decreaseKey(s);
		}
	}
	
	void calculateScore(State &state) {
		state.score.h = heuristics->evaluate(&state);
		//state.score.f = state.score.h;
		state.score.f = state.score.g + state.score.h;
	}
	
	const State *popOpen() {
		return open.pop();
	}
	
	vector<State> traceback(const State *state) {
		vector<State> trace;
		for (const State *s = state; s != NULL; s = s->parent)
			trace.emplace_back(*s);
		
		return trace;
	}
	
	void appendNeighbours(const State *state) {
		constexpr Direction allDirections[] = {Direction::Top, Direction::Left, Direction::Right, Direction::Bottom};
		
		for (auto direction : allDirections) {
			if (isOppositeDirection(state, direction))
				continue;
			
			optional<State> neighbour = state->getNeighbour(direction);
			if (neighbour != nullopt)
				pushOrUpdateOpenAndCloseDuplicates(*neighbour);
		}
	}
	
	bool isOppositeDirection(const State *state, Direction direction) {
		return (state->parent != NULL) && (
			(state->parentDirection == Direction::Top && direction == Direction::Bottom) ||
			(state->parentDirection == Direction::Left && direction == Direction::Right) ||
			(state->parentDirection == Direction::Right && direction == Direction::Left) ||
			(state->parentDirection == Direction::Bottom && direction == Direction::Top)
	   );
	}
};

#endif
