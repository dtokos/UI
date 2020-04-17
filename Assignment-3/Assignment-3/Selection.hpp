#ifndef Selection_hpp
#define Selection_hpp

#include <vector>
#include "Agent.hpp"

using namespace std;

struct Selection {
	virtual void prepare(vector<Agent> &population) = 0;
	virtual Agent& pick(vector<Agent> &population, float chance) = 0;
};

struct RouletteSelection : public Selection {
	void prepare(vector<Agent> &population) {
		fitnessSum = 0;
		for (const auto &a : population)
			fitnessSum += a.fitness;
	}
	
	Agent& pick(vector<Agent> &population, float chance) {
		chance *= fitnessSum;
		int i = 0;
		for (; i < population.size() - 1; i++) {
			chance -= population[i].fitness;
			
			if (chance <= 0.0)
				return population[i];
		}
		
		return population[i];
	}
	
private:
	float fitnessSum;
};

#endif
