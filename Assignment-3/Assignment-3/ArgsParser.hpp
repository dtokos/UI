#ifndef ArgsParser_hpp
#define ArgsParser_hpp

#include <docopt/docopt.h>
#include "Evolution.hpp"

using namespace std;

// TODO: Improve error handling

class ArgsParser {
public:
	struct Args {
		Evolution::Config config;
		Map map;
		int populationLimit;
		int populationSize;
	};
	
	Args parse(int argc, const char *argv[]);
private:
	map<string, docopt::value> args;
	
	Evolution::Config parseConfig();
	Fitness *parseFitness();
	Mutation *parseMutation(Random *rnd);
	VirtualMachine parseVirtualMachine();
	
	Map parseMap();
	
	template<typename T>
	T parseArg(const string &name);
	float parseChance(const string &name);
	int parseInt(const string &name, int min);
	
	Map::Vector parseVector(const string &value, const string &delimiter, const Map::Vector &min, const Map::Vector &max);
	Map::Vector parseVector(const string &value, const string &delimiter, const Map::Vector &min);
	Map::Vector parseVector(const string &value, const string &delimiter);
	vector<Map::Vector> parseTreasures(const Map::Vector &size);
	vector<string> split(const string &value, const string &delimiter);
};

#endif
