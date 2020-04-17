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
	struct Config {
		Fitness *fitness;
		Selection *selection;
		Crossing *generator;
		Mutation *mutation;
		VirtualMachine vm;
	};
	
	Evolution(const Config &c) :
		fitness(c.fitness),
		selection(c.selection),
		generator(c.generator),
		mutation(c.mutation),
		vm(c.vm) {}
	// TODO: Refactor rnd generation
	// TODO: Change return type
	void start(const Map &map, int populationSize, int populationLimit);
	
private:
	Fitness *fitness;
	Selection *selection;
	Crossing *generator;
	Mutation *mutation;
	
	VirtualMachine vm;
	vector<Agent> population, nextPopulation;
	
	void createRandomPopulation(int populationSize);
	optional<Agent> executeAndCalculateFitnesses(const Map &map);
	void pickParentsAndCreateChildren();
	void mutateChildren();
	void swapPopulation();
	// TODO: Change return type
	void currentlyBestAgent();
};

#endif
