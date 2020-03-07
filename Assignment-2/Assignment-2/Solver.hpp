#ifndef Solver_hpp
#define Solver_hpp

#include "./Heuristics.hpp"

class Solver {
public:
	struct SolvingException : public exception {
		string message;
		SolvingException(string message) : message(message) {};
		const char* what() const throw() { return message.c_str(); }
	};
	
	// Return type will be different
	void solve(State &start, State &finish) {
		if (start.size != finish.size)
			throw SolvingException("State sizes doesnt match: " + start.size.toString() + " and " + finish.size.toString());
		
		throw SolvingException("Solution doesnt exist");
	}
};

#endif
