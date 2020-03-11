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
	
	vector<State> solve(State &start, State &finish) {
		if (start.size != finish.size)
			throw SolvingException("State sizes doesnt match: " + start.size.toString() + " and " + finish.size.toString());
		
		clear();
		return findSolution(start, finish);
	}
	
private:
	struct HeuristicCompare {
		bool operator()(const State *lhs, const State *rhs) const {
			return lhs->score.f > rhs->score.f;
		}
	};
	
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
	priority_queue<const State *, vector<const State *>, HeuristicCompare> open;
	
	void clear() {
		open = priority_queue<const State *, vector<const State *>, HeuristicCompare>();
		states.clear();
	}
	
	vector<State> findSolution(State &start, State &finish) {
		start.score.g = 0;
		heuristics->setFinal(&finish);
		pushOpenAndCloseDuplicates(start);
		
		while (!open.empty()) {
			const State *state = popOpen();
			
			if (*state == finish)
				return traceback(state);
			
			appendNeighbours(state);
		}
		
		throw SolvingException("Solution doesnt exist");
	}
	
	void pushOpenAndCloseDuplicates(State &state) {
		calculateScore(state);
		open.push(closeDuplicates(state));
	}
	
	void calculateScore(State &state) {
		state.score.h = heuristics->evaluate(&state);
		state.score.f = state.score.g + state.score.h;
	}
	
	const State *closeDuplicates(State &state) {
		return &(*states.insert(state).first);
	}
	
	const State *popOpen() {
		const State *s = open.top();
		open.pop();
		return s;
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
			if (neighbour != nullopt && isNotDuplicate(*neighbour))
				pushOpenAndCloseDuplicates(*neighbour);
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
	
	bool isNotDuplicate(const State &state) {
		return states.find(state) == states.end();
	}
};

#endif
