#ifndef ArgsParser_hpp
#define ArgsParser_hpp

#include <docopt/docopt.h>
#include "Evolution.hpp"

using namespace std;

class ArgsParser {
public:
	struct Args {
		Evolution::Config config;
		Map map;
		int populationLimit;
		int populationSize;
	};
	
	struct ParsingError : public exception {
		ParsingError(const string &m) : message(m) {}
		
		static ParsingError invalid(const string &type, const string &name) {
			return ParsingError(type + " '" + name + "' is invalid");
		}
		
		static ParsingError outOfRange(const string &type, const string &name) {
			return ParsingError(type + " '" + name + "' is out of range");
		}
		
		virtual const char *what() const noexcept {
			return message.c_str();
		}
		
		const string message;
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
	float parseFloat(const string &name, float min, float max);
	int parseInt(const string &name, int min);
	
	Map::Vector parseVector(const string &name, const string &delimiter, const Map::Vector &min);
	Map::Vector parseVector(const string &name, const string &delimiter, const Map::Vector &min, const Map::Vector &max);
	Map::Vector parseVector(const string &name, const string &delimiter);
	Map::Vector parseVectorValue(const string &value, const string &delimiter);
	Map::Vector checkVectorSize(const Map::Vector &vector, const Map::Vector &min);
	Map::Vector checkVectorSize(const Map::Vector &vector, const Map::Vector &min, const Map::Vector &max);
	
	vector<Map::Vector> parseTreasures(const Map::Vector &size);
	vector<string> split(const string &value, const string &delimiter);
};

#endif
