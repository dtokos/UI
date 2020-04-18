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
		throw invalid_argument(name);
}

template<>
int ArgsParser::parseArg<int>(const string &name) {
	try {
		return stoi(args[name].asString());
	} catch (const invalid_argument &e) {
		throw invalid_argument(name);
	} catch (const out_of_range &e) {
		throw out_of_range(name);
	}
}

template<>
float ArgsParser::parseArg<float>(const string &name) {
	try {
		return stof(args[name].asString());
	} catch (const invalid_argument &e) {
		throw invalid_argument(name);
	} catch (const out_of_range &e) {
		throw out_of_range(name);
	}
}

template<>
string ArgsParser::parseArg<string>(const string &name) {
	return args[name].asString();
}

float ArgsParser::parseFloat(const string &name, float min, float max) {
	float chance = parseArg<float>(name);
	if (chance < min || chance > max)
		throw out_of_range(name);
	
	return chance;
}

int ArgsParser::parseInt(const string &name, int min) {
	int value = parseArg<int>(name);
	if (value < min)
		throw out_of_range(name);
	
	return value;
}

ArgsParser::Args ArgsParser::parse(int argc, const char *argv[]) {
	try {
		args = docopt::docopt(USAGE, {argv + 1, argv + argc}, true);
		
		return Args {
			parseConfig(),
			parseMap(),
			parseInt("--ev_pl", 1),
			parseInt("--ev_ps", 1),
		};
	} catch (const invalid_argument &e) {
		throw ParsingError::invalid("Option", e.what());
	} catch (const out_of_range &e) {
		throw ParsingError::outOfRange("Option", e.what());
	}
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
	try {
		return new TreasureDistanceFitness(parseFloat("--tdf_sw", 0.0, 1.0));
	} catch (const out_of_range &e) {
		throw ParsingError::outOfRange("Option", e.what());
	} catch (const invalid_argument &e) {
		throw ParsingError::invalid("Option", e.what());
	}
}

Mutation *ArgsParser::parseMutation(Random *rnd) {
	try {
		return new InvertMutation(rnd, parseFloat("--im_limit", 0.0, 1.0));
	} catch (const out_of_range &e) {
		throw ParsingError::outOfRange("Option", e.what());
	} catch (const invalid_argument &e) {
		throw ParsingError::invalid("Option", e.what());
	}
}

VirtualMachine ArgsParser::parseVirtualMachine() {
	try {
		return VirtualMachine{parseArg<bool>("--vm_stae"), parseInt("--vm_il", 1)};
	} catch (const out_of_range &e) {
		throw ParsingError::outOfRange("Option", e.what());
	} catch (const invalid_argument &e) {
		throw ParsingError::invalid("Option", e.what());
	}
}

Map ArgsParser::parseMap() {
	try {
		const Map::Vector size = parseVector("<map_dimension>", "x", {1, 1});
		const Map::Vector start = parseVector("<start>", ",", {1, 1}, size);
		
		return Map {
			size,
			Map::Vector{start.x - 1, start.y - 1},
			parseTreasures(size),
		};
	} catch (const invalid_argument &e) {
		throw ParsingError::invalid("Argument", e.what());
	} catch (const out_of_range &e) {
		throw ParsingError::outOfRange("Argument", e.what());
	}
}

Map::Vector ArgsParser::parseVector(const string &name, const string &delimiter, const Map::Vector &min) {
	try {
		return checkVectorSize(parseVector(name, delimiter), min);
	} catch (const invalid_argument &e) {
		throw invalid_argument(name);
	} catch (const out_of_range &e) {
		throw out_of_range(name);
	}
}

Map::Vector ArgsParser::parseVector(const string &name, const string &delimiter, const Map::Vector &min, const Map::Vector &max) {
	try {
		return checkVectorSize(parseVector(name, delimiter), min, max);
	} catch (const invalid_argument &e) {
		throw invalid_argument(name);
	} catch (const out_of_range &e) {
		throw out_of_range(name);
	}
}

Map::Vector ArgsParser::parseVector(const string &name, const string &delimiter) {
	return parseVectorValue(parseArg<string>(name), delimiter);
}

vector<Map::Vector> ArgsParser::parseTreasures(const Map::Vector &size) {
	constexpr char arg[] = "<treasure>";
	
	try {
		vector<string> treasureArgs = args[arg].asStringList();
		vector<Map::Vector> treasures;
		
		for (const auto &treasure : treasureArgs) {
			const Map::Vector t = checkVectorSize(parseVectorValue(treasure, ","), {1, 1}, size);
			treasures.emplace_back(Map::Vector{t.x - 1, t.y - 1});
		}
		
		return treasures;
	} catch (const invalid_argument &e) {
		throw ParsingError::invalid("Argument", arg);
	} catch (const out_of_range &e) {
		throw ParsingError::outOfRange("Argument", arg);
	}
}

Map::Vector ArgsParser::parseVectorValue(const string &value, const string &delimiter) {
	vector<string> parts = split(value, delimiter);
	if (parts.size() != 2)
		throw invalid_argument("Invalid vector value");
	
	return Map::Vector{stoi(parts[0]), stoi(parts[1])};
}

Map::Vector ArgsParser::checkVectorSize(const Map::Vector &vec, const Map::Vector &min) {
	if (vec.x < min.x || vec.y < min.y)
		throw out_of_range("Vector is out of range");
	
	return vec;
}

Map::Vector ArgsParser::checkVectorSize(const Map::Vector &vec, const Map::Vector &min, const Map::Vector &max) {
	if (vec.x < min.x || vec.y < min.y || vec.x > max.x || vec.y > max.y)
		throw out_of_range("Vector is out of range");
	
	return vec;
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
