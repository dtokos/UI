#include "ArgsParser.hpp"

static const char USAGE[] =
R"(Assignment.

Geneteic algorithm...

Usage:
  assignment <start> <map_dimension> <treasure>... [options]
  assignment (-h | --help)

Arguments:
  <start>            Determines starting position of player.
                     Expects string with format 'A,B' where A and B are from N+.
  
  <map_dimension>    Determines size of map.
                     Expects string with format 'AxB' where A and B are from N+.
  
  <treasure>         Determines position of treasure on the map.
                     Expects string with format 'A,B' where A and B are from N+.

Examples:
  assignment 0,0 5x5 1,2 3,4 4,2
  assignment 0,0 5x5 1,2 3,4 4,2 --im_limit=20

Options:
  -h --help          Show this screen.
  
  --tdf_sw=SW        Configures step weight for TreasureDistanceFitness.
                     Each step will decrease fitness by small amount.
                     [default: 0.001]
  
  --im_limit=L       Configures mutation chance limit for InvertMutation.
                     Only chances greater than this limit will mutate.
                     [default: 0.7]
  
  --vm_stae=STAE     Configures virtual machine behaviour of VirtualMachine at end of program.
                     If set to true, then the execution will end.
                     Otherwise the program will jump back to start and continue execution.
                     [default: true]
  
  --vm_il=IL         Configures instruction limit of VirtualMachine.
                     If this limit is reached, then the execution will stop.
                     [default: 500]
  
  --ev_pl=PL         Configures population limit of Evolution.
                     If this limit is reached, then the currently best agent will be returned.
                     [default: 100]
  
  --ev_ps=PS         Configures population size of Evolution.
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

ArgsParser::Args ArgsParser::parse(int argc, const char *argv[]) {
	args = docopt::docopt(USAGE, {argv + 1, argv + argc}, true);
	
	return Args {
		parseConfig(),
		parseMap(),
		parseArg<int>("--ev_pl"),
		parseArg<int>("--ev_ps"),
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
	return new TreasureDistanceFitness(parseArg<float>("--tdf_sw"));
}

Mutation *ArgsParser::parseMutation(Random *rnd) {
	return new InvertMutation(rnd, parseArg<float>("--im_limit"));
}

VirtualMachine ArgsParser::parseVirtualMachine() {
	return VirtualMachine{parseArg<bool>("--vm_stae"), parseArg<int>("--vm_il")};
}

Map ArgsParser::parseMap() {
	return Map {
		parseVector(parseArg<string>("<map_dimension>"), "x"),
		parseVector(parseArg<string>("<start>"), ","),
		parseTreasures(),
	};
}

Map::Vector ArgsParser::parseVector(const string &value, const string &delimiter) {
	vector<string> parts = split(value, delimiter);
	if (parts.size() != 2)
		throw invalid_argument("Invalid number of arguments passed to vector");
		
	return Map::Vector{stoi(parts[0]), stoi(parts[1])};
}

vector<Map::Vector> ArgsParser::parseTreasures() {
	vector<string> treasureArgs = args["<treasure>"].asStringList();
	vector<Map::Vector> treasures;
	
	for (const auto &treasure : treasureArgs)
		treasures.emplace_back(parseVector(treasure, ","));
	
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
