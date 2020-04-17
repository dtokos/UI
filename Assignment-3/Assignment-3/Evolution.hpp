#ifndef Evolution_hpp
#define Evolution_hpp

#include <vector>
#include <optional>
#include "Map.hpp"
#include "Program.hpp"
#include "VirtualMachine.hpp"
#include "Agent.hpp"
#include "Fitness.hpp"
#include "Selection.hpp"
#include "Crossing.hpp"
#include "Mutation.hpp"

using namespace std;

class Evolution {
public:
	// TODO: Change return type
	void start(const Map &map, int populationSize, int populationLimit);
	
private:
	Fitness *fitness;
	Selection *selection;
	Crossing *generator;
	Mutation *mutation;
	
	VirtualMachine vm;
	vector<Agent> population, nextPopulation;
	int populationCount, populationLimit;
	
	void createRandomPopulation(int populationSize);
	optional<Agent> executeAndCalculateFitnesses();
	void pickParentsAndCreateChildren();
	void mutateChildren();
	void swapPopulation();
	// TODO: Change return type
	void currentlyBestAgent();
};

#endif
