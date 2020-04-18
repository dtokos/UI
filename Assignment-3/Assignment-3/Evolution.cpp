#include "Evolution.hpp"

void Evolution::start(const Map &m, int ps, int pl) {
	// Create random population (min 20)
	createRandomPopulation(ps);
	
	// Loop
	for (int populationCount = 0; populationCount < pl; populationCount++) {
		// Calculate fitness for every agent
		auto result = executeAndCalculateFitnesses(m);
		// If agent collected all treasures then return
		if (result != nullopt)
			return; // TODO: Change return type
		// Find parents and create children
		pickParentsAndCreateChildren();
		// Mutate children
		mutateChildren();
		// Swap population
		swapPopulation();
	}
	
	// If reached population limit then return currently best agent
	return currentlyBestAgent();
}

void Evolution::createRandomPopulation(int ps) {
	population.clear();
	for (int i = 0; i < ps; i++)
		population.emplace_back(Agent{Program::random(), -1.0f});
}

optional<Agent> Evolution::executeAndCalculateFitnesses(const Map &m) {
	for (auto &agent : population) {
		auto r = vm.execute<false>(agent.program, m);
		
		if (r.termination == VirtualMachine::Termination::Success)
			return agent;
		
		agent.fitness = fitness->calculate(r);
	}
	
	return nullopt;
}

void Evolution::pickParentsAndCreateChildren() {
	selection->prepare(population);
	nextPopulation.clear();
	
	for (int i = 0; i < population.size(); i++)
		nextPopulation.emplace_back(crossing->cross(selection->pick(population, random->chance()), selection->pick(population, random->chance())));
}

void Evolution::mutateChildren() {
	mutation->mutate(nextPopulation);
}

void Evolution::swapPopulation() {
	population = nextPopulation;
}

void Evolution::currentlyBestAgent() {
	//*max_element(population.begin(), population.end(), [](const auto &a, const auto &b) {return a.fitness < b.fitness;});
}
