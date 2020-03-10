#ifndef ArgsParser_hpp
#define ArgsParser_hpp

#include "./Heuristics.hpp"

using namespace std;

class ArgsParser {
public:
	struct Args {
		State start;
		State finish;
		Heuristics *heuristics;
	};
	
	struct ParsingException : public exception {
		string message;
		ParsingException(string message) : message(message) {};
		const char* what() const throw() { return message.c_str(); }
	};
	
	
	Args parse(vector<string> args) {
		checkNumOfArgs(args);
		State::Size size = parseSize(args[2]);
		
		return {
			parseState(args[3], size),
			parseState(args[4], size),
			parseHeuristics(args[1])
		};
	}
	
private:
	void checkNumOfArgs(const vector<string> &args) {
		if (args.size() < 5)
			throw ParsingException("Too few arguments passed: " + to_string(args.size()) + " minimum: 5");
	}
	
	State::Size parseSize(const string &sizeStr) {
		try {
			State::Size size = tryParsingSize(sizeStr);
			if (size.width <= 1 || size.height <= 1)
				throw ParsingException("Invalid state size: " + size.toString() + " minimum: (1, 1)");
			
			return size;
		} catch (const invalid_argument &e) {
			throw ParsingException("Invalid state size: " + sizeStr);
		} catch (const out_of_range &e) {
			throw ParsingException("Invalid state size: " + sizeStr);
		}
	}
	
	State::Size tryParsingSize(const string &size) {
		vector<string> parts = split(size, "x");
		if (parts.size() != 2)
			throw ParsingException("Invalid state size: " + size);
		
		return {stoi(parts[0]), stoi(parts[1])};
	}
	
	vector<string> split(const string &value, const string &delimiter) {
		size_t position = 0, offset = 0;
		vector<string> result;
		
		while ((position = value.find(delimiter, offset)) != string::npos) {
			result.push_back(value.substr(offset, position - offset));
			offset = position + 1;
		}
		
		result.push_back(value.substr(offset));
		
		return result;
	}
	
	State parseState(const string stateStr, const State::Size &size) {
		try {
			State s = tryParsingState(stateStr, size);
			if (s.tiles.size() != s.size.width * s.size.height)
				throw ParsingException("Invalid number of tiles: " + to_string(s.tiles.size()) + " expected: " + to_string(s.size.width * s.size.height));
			
			return s;
		} catch (const invalid_argument &e) {
			throw ParsingException("Invalid state: " + stateStr);
		} catch (const out_of_range &e) {
			throw ParsingException("Invalid state: " + stateStr);
		}
	}
	
	State tryParsingState(const string &stateStr, const State::Size size) {
		vector<string> parts = split(stateStr, ",");
		vector<int> tiles;
		State::Position emptyPos;
		
		bool hasEmpty = false;
		int index = 0;
		for (auto &part : parts) {
			if (part == "_") {
				emptyPos.x = index % size.width;
				emptyPos.y = index / size.height;
				
				if (hasEmpty)
					throw ParsingException("Duplicate empty tile at: " + emptyPos.toString());
				else {
					tiles.push_back(-1);
					hasEmpty = true;
				}
			} else
				tiles.push_back(stoi(part));
			
			index++;
		}
	
		return {size, tiles, emptyPos};
	}
	
	Heuristics *parseHeuristics(const string &hStr) {
		if (hStr == "1")
			return new SpotHeuristics();
		else if (hStr == "2")
			return new DistanceHeuristics();
		else if (hStr == "3")
			return new SumHeuristics({new SpotHeuristics(), new DistanceHeuristics()});
		
		throw ParsingException("Invalid heuristics: " + hStr + " valid options are: 1,2,3");
	}
};

#endif
