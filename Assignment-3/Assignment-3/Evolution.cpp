#include "Evolution.hpp"

Evolution::Result Evolution::start(const Map &m, int ps, int pl) {
	createRandomPopulation(ps);
	
	for (int populationCount = 0; populationCount < pl; populationCount++) {
		auto result = executeAndCalculateFitnesses(m, populationCount + 1);
		
		if (result != nullopt)
			return *result;
		
		pickParentsAndCreateChildren();
		mutateChildren();
		swapPopulation();
	}
	
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
