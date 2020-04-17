#ifndef Mutation_hpp
#define Mutation_hpp

#include <vector>
#include "Agent.hpp"

using namespace std;

struct Mutation {
	virtual void mutate(vector<Agent> &population) = 0;
	virtual void mutateInstruction(Instruction &instruction, float chance) = 0;
};

struct InvertMutation : public Mutation {
	InvertMutation(float l = 0.7) : limit(l) {}
	
	void mutate(vector<Agent> &population) {
		for (auto &a : population)
			for (auto &i : a.program.instructions)
				if (rand() / static_cast<float>(RAND_MAX) > limit)
					mutateInstruction(i, rand() / static_cast<float>(RAND_MAX));
	}
	
	virtual void mutateInstruction(Instruction &instruction, float chance) {
		instruction.raw ^= 1 << (uint8_t)(chance * 7);
	}

private:
	float limit;
};

#endif
