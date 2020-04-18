#include <iostream>
#include "Evolution.hpp"
#include "ArgsParser.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
	ArgsParser parser;
	ArgsParser::Args args = parser.parse(argc, argv);
	Evolution e{args.config};
	
	e.start(args.map, args.populationSize, args.populationLimit);
	
	return 0;
}
