#include "Evolution.hpp"

Evolution::Result Evolution::start(const Map &m, int ps, int pl) {
	// Create random population (min 20)
	createRandomPopulation(ps);
	
	// Loop
	for (int populationCount = 0; populationCount < pl; populationCount++) {
		// Calculate fitness for every agent
		auto result = executeAndCalculateFitnesses(m, populationCount);
		// If agent collected all treasures then return
		if (result != nullopt)
			return *result;
		// Find parents and create children
		pickParentsAndCreateChildren();
		// Mutate children
		mutateChildren();
		// Swap population
		swapPopulation();
	}
	
	// If reached population limit then return currently best agent
	return currentlyBestAgent(m, pl);
}

void Evolution::createRandomPopulation(int ps) {
	population.clear();
	for (int i = 0; i < ps; i++)
		population.emplace_back(Agent{Program::random(), -1.0f});
}

optional<Evolution::Result> Evolution::executeAndCalculateFitnesses(const Map &m, int pn) {
	for (auto &agent : population) {
		auto r = vm.execute<false>(agent.program, m);
		
		if (r.termination == VirtualMachine::Termination::Success)
			return Result{true, pn, agent.program, r.path};
		
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

Evolution::Result Evolution::currentlyBestAgent(const Map &m, int pl) {
	const auto &agent = *max_element(population.begin(), population.end(), [](const auto &a, const auto &b) {return a.fitness < b.fitness;});
	const auto result = vm.execute<false>(agent.program, m);
	return Result{false, pl, agent.program, result.path};
}
