#ifndef ScoreCalculator_hpp
#define ScoreCalculator_hpp

#include "./Heuristics.hpp"

class ScoreCalculator {
public:
	ScoreCalculator(Heuristics *h) : heuristics(h) {}
	virtual void calculate(State *curentS) = 0;
	virtual void setFinal(State *s) {
		heuristics->setFinal(s);
	}

protected:
	Heuristics *heuristics;
};

class GHCalculator : public ScoreCalculator {
public:
	using ScoreCalculator::ScoreCalculator;
	
	void calculate(State *curentS) {
		curentS->score.h = heuristics->evaluate(curentS);
		curentS->score.f = curentS->score.g + curentS->score.h;
	}
};

class HCalculator : public ScoreCalculator {
public:
	using ScoreCalculator::ScoreCalculator;
	
	void calculate(State *curentS) {
		curentS->score.h = heuristics->evaluate(curentS);
		curentS->score.f = curentS->score.h;
	}
};

#endif
