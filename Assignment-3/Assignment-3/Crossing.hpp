#ifndef Crossing_hpp
#define Crossing_hpp

#include "Random.hpp"
#include "Agent.hpp"

using namespace std;

struct Crossing {
	virtual Agent cross(const Agent &parentA, const Agent &parentB) = 0;
	virtual Instruction crossInstruction(const Instruction &parentA, const Instruction &parentB, float chance) = 0;
};

struct SliceCrossing : public Crossing {
	SliceCrossing(Random *rnd) : rnd(rnd) {}
	
	Agent cross(const Agent &pa, const Agent &pb) {
		Agent child;
		child.fitness = -1;
		
		for (int i = 0; i < pa.program.size(); i++)
			child.program[i] = crossInstruction(pa.program[i], pb.program[i], rnd->chance());
		
		return child;
	}
	
	Instruction crossInstruction(const Instruction &pa, const Instruction &pb, float chance) {
		uint8_t mask = ~0 << (uint8_t)(chance * 8);
		return Instruction{static_cast<uint8_t>((pa.raw & mask) | (pb.raw & ~mask))};
	}
	
private:
	Random *rnd;
};

#endif
