#ifndef Evolution_hpp
#define Evolution_hpp

#include <vector>
#include <optional>
#include "Random.hpp"
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
		Random *random;
		Fitness *fitness;
		Selection *selection;
		Crossing *generator;
		Mutation *mutation;
		VirtualMachine vm;
	};
	
	Evolution(const Config &c) :
		random(c.random),
		fitness(c.fitness),
		selection(c.selection),
		generator(c.generator),
		mutation(c.mutation),
		vm(c.vm) {}
	// TODO: Change return type
	void start(const Map &map, int populationSize, int populationLimit);
	
private:
	Random *random;
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
