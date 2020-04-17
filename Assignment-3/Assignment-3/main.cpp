#include <iostream>
#include "Evolution.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
	TreasureDistanceFitness f;
	RouletteSelection s;
	SliceCrossing g;
	InvertMutation m;
	VirtualMachine vm{false, 100};
	Evolution::Config c {&f, &s, &g, &m, vm};
	Evolution e{c};
	
	Map map{{5, 5}, {0, 0}, {{4, 4}}};
	
	e.start(map, 20, 10);
	
	return 0;
}
