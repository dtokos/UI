#ifndef Mutation_hpp
#define Mutation_hpp

#include <vector>
#include "Random.hpp"
#include "Agent.hpp"

using namespace std;

struct Mutation {
	virtual void mutate(vector<Agent> &population) = 0;
	virtual void mutateInstruction(Instruction &instruction, float chance) = 0;
};

struct InvertMutation : public Mutation {
	InvertMutation(Random *rnd, float l = 0.7) : rnd(rnd), limit(l) {}
	
	void mutate(vector<Agent> &population) {
		for (auto &a : population)
			for (auto &i : a.program.instructions)
				if (rnd->chance() > limit)
					mutateInstruction(i, rnd->chance());
	}
	
	virtual void mutateInstruction(Instruction &instruction, float chance) {
		instruction.raw ^= 1 << (uint8_t)(chance * 7);
	}

private:
	Random *rnd;
	float limit;
};

#endif
