#ifndef Fitness_hpp
#define Fitness_hpp

#include "VirtualMachine.hpp"

struct Fitness {
	virtual float calculate(const VirtualMachine::Result<false> &result) = 0;
};

struct TreasureDistanceFitness : public Fitness {
	TreasureDistanceFitness(float sw = 0.001) : stepWeight(sw) {}
	
	float calculate(const VirtualMachine::Result<false> &result) {
		return 1.0 + (result.collectedTreasures - result.instructionsExecuted * stepWeight);
	}

private:
	float stepWeight;
};

#endif
