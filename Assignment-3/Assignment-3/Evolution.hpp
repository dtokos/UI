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
		Crossing *crossing;
		Mutation *mutation;
		VirtualMachine vm;
	};
	
	struct Result {
		bool success;
		int populationNumber;
		Program program;
		vector<Instruction::Direction> path;
	};
	
	Evolution(const Config &c) :
		random(c.random),
		fitness(c.fitness),
		selection(c.selection),
		crossing(c.crossing),
		mutation(c.mutation),
		vm(c.vm) {}
	
	Result start(const Map &map, int populationSize, int populationLimit);
	
private:
	Random *random;
	Fitness *fitness;
	Selection *selection;
	Crossing *crossing;
	Mutation *mutation;
	
	VirtualMachine vm;
	vector<Agent> population, nextPopulation;
	
	void createRandomPopulation(int populationSize);
	optional<Evolution::Result> executeAndCalculateFitnesses(const Map &map, int populationNumber);
	void pickParentsAndCreateChildren();
	void mutateChildren();
	void swapPopulation();
	Evolution::Result currentlyBestAgent(const Map &map, int populationLimit);
};

#endif
