#include "ArgsParser.hpp"

static const char USAGE[] =
R"(Assignment.

Geneteic algorithm...

Usage:
  assignment <start> <map_dimension> <treasure>... [options]
  assignment (-h | --help)

Arguments:
  <start>            Determines starting position of player.
                     Expects string with format 'A,B' where A and B are from <1, map_dimension>.
  
  <map_dimension>    Determines size of map.
                     Expects string with format 'AxB' where A and B are from N+.
  
  <treasure>         Determines position of treasure on the map.
                     Expects string with format 'A,B' where A and B are from <1, map_dimension>.

Examples:
  assignment 0,0 5x5 1,2 3,4 4,2
  assignment 0,0 5x5 1,2 3,4 4,2 --im_limit=20

Options:
  -h --help          Show this screen.
  
  --tdf_sw=SW        Configures step weight for TreasureDistanceFitness.
                     Each step will decrease fitness by small amount.
                     Accepts values from range <0.0, 1.0>.
                     [default: 0.001]
  
  --im_limit=L       Configures mutation chance limit for InvertMutation.
                     Only chances greater than this limit will mutate.
                     Accepts values from range <0.0, 1.0>.
                     [default: 0.7]
  
  --vm_stae=STAE     Configures virtual machine behaviour of VirtualMachine at end of program.
                     If set to true, then the execution will end.
                     Otherwise the program will jump back to start and continue execution.
                     Accepts values (true|false|1|0).
                     [default: true]
  
  --vm_il=IL         Configures instruction limit of VirtualMachine.
                     If this limit is reached, then the execution will stop.
                     Accepts values from N+.
                     [default: 500]
  
  --ev_pl=PL         Configures population limit of Evolution.
                     If this limit is reached, then the currently best agent will be returned.
                     Accepts values from N+.
                     [default: 100]
  
  --ev_ps=PS         Configures population size of Evolution.
                     Accepts values from N+.
                     [default: 20]
)";

template<>
bool ArgsParser::parseArg<bool>(const string &name) {
	const auto& arg = args[name].asString();
	if (arg == "true" || arg == "1")
		return true;
	else if (arg == "false" || arg == "0")
		return false;
	else
		throw invalid_argument("Argument " + name + " is not boolean");
}

template<>
int ArgsParser::parseArg<int>(const string &name) {
	const auto& arg = args[name].asString();
	try {
		return stoi(arg);
	} catch (const invalid_argument &e) {
		throw invalid_argument("Argument " + name + " is not int");
	} catch (const out_of_range &e) {
		throw invalid_argument("Argument " + name + " is out of int range");
	}
}

template<>
float ArgsParser::parseArg<float>(const string &name) {
	const auto& arg = args[name].asString();
	try {
		return stof(arg);
	} catch (const invalid_argument &e) {
		throw invalid_argument("Argument " + name + " is not float");
	} catch (const out_of_range &e) {
		throw invalid_argument("Argument " + name + " is out of float range");
	}
}

template<>
string ArgsParser::parseArg<string>(const string &name) {
	return args[name].asString();
}

float ArgsParser::parseChance(const string &name) {
	float chance = parseArg<float>(name);
	if (chance < 0.0 || chance > 1.0)
		throw range_error("Argument is out of range");
	
	return chance;
}

int ArgsParser::parseInt(const string &name, int min) {
	int value = parseArg<int>(name);
	if (value < min)
		throw range_error("Argument is out of range");
	
	return value;
}

ArgsParser::Args ArgsParser::parse(int argc, const char *argv[]) {
	args = docopt::docopt(USAGE, {argv + 1, argv + argc}, true);
	
	return Args {
		parseConfig(),
		parseMap(),
		parseInt("--ev_pl", 1),
		parseInt("--ev_ps", 1),
	};
}

Evolution::Config ArgsParser::parseConfig() {
	Random *rnd = new RandRandom();
	
	return Evolution::Config{
		rnd,
		parseFitness(),
		new RouletteSelection(),
		new SliceCrossing(rnd),
		parseMutation(rnd),
		parseVirtualMachine()
	};
}

Fitness *ArgsParser::parseFitness() {
	return new TreasureDistanceFitness(parseChance("--tdf_sw"));
}

Mutation *ArgsParser::parseMutation(Random *rnd) {
	return new InvertMutation(rnd, parseChance("--im_limit"));
}

VirtualMachine ArgsParser::parseVirtualMachine() {
	return VirtualMachine{parseArg<bool>("--vm_stae"), parseInt("--vm_il", 1)};
}

Map ArgsParser::parseMap() {
	const Map::Vector size = parseVector(parseArg<string>("<map_dimension>"), "x", {1, 1});
	const Map::Vector start = parseVector(parseArg<string>("<start>"), ",", {1, 1}, size);
	
	return Map {
		size,
		Map::Vector{start.x - 1, start.y - 1},
		parseTreasures(size),
	};
}

Map::Vector ArgsParser::parseVector(const string &value, const string &delimiter, const Map::Vector &min, const Map::Vector &max) {
	Map::Vector vec = parseVector(value, delimiter);
	
	if (vec.x < min.x || vec.y < min.y || vec.x > max.x || vec.y > max.y)
		throw range_error("Argument is out of range");
	
	return vec;
}

Map::Vector ArgsParser::parseVector(const string &value, const string &delimiter, const Map::Vector &min) {
	Map::Vector vec = parseVector(value, delimiter);
	
	if (vec.x < min.x || vec.y < min.y)
		throw range_error("Argument is out of range");
	
	return vec;
}

Map::Vector ArgsParser::parseVector(const string &value, const string &delimiter) {
	vector<string> parts = split(value, delimiter);
	if (parts.size() != 2)
		throw invalid_argument("Invalid number of arguments passed to vector");
		
	return Map::Vector{stoi(parts[0]), stoi(parts[1])};
}

vector<Map::Vector> ArgsParser::parseTreasures(const Map::Vector &size) {
	vector<string> treasureArgs = args["<treasure>"].asStringList();
	vector<Map::Vector> treasures;
	
	for (const auto &treasure : treasureArgs) {
		const Map::Vector t = parseVector(treasure, ",", {1, 1}, size);
		treasures.emplace_back(Map::Vector{t.x - 1, t.y - 1});
	}
	
	return treasures;
}

vector<string> ArgsParser::split(const string &value, const string &delimiter) {
	size_t position = 0, offset = 0;
	vector<string> result;
	
	while ((position = value.find(delimiter, offset)) != string::npos) {
		result.push_back(value.substr(offset, position - offset));
		offset = position + 1;
	}
	
	result.push_back(value.substr(offset));
	
	return result;
}
