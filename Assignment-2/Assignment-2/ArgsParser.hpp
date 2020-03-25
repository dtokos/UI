#ifndef ArgsParser_hpp
#define ArgsParser_hpp

#include <set>
#include "./ScoreCalculator.hpp"

using namespace std;

class ArgsParser {
public:
	struct Args {
		State start;
		State finish;
		ScoreCalculator *scoreCalculator;
	};
	
	struct Config {
		string tileDelimiter;
		string emptyTile;
	};
	
	struct ParsingException : public exception {
		string message;
		ParsingException(string message) : message(message) {};
		const char* what() const throw() { return message.c_str(); }
	};
	
	ArgsParser(Config c) : config(c) {}
	
	Args parse(vector<string> args) {
		checkNumOfArgs(args);
		State::Size size = parseSize(args[3]);
		State start = parseState(args[4], size);
		State finish = parseState(args[5], size);
		checkSameStateElements(start, finish);
		
		return {
			start,
			finish,
			parseScoreCalculator(args[2], parseHeuristics(args[1])),
		};
	}
	
private:
	Config config;
	
	void checkNumOfArgs(const vector<string> &args) {
		if (args.size() < 6)
			throw ParsingException("Too few arguments passed: " + to_string(args.size()) + " minimum: 6");
	}
	
	void checkSameStateElements(const State &start, const State &finish) {
		vector<uint8_t> intersection;
		set<uint8_t> ss{start.tiles.begin(), start.tiles.end()};
		set<uint8_t> sf{finish.tiles.begin(), finish.tiles.end()};
		set_intersection(ss.begin(), ss.end(), sf.begin(), sf.end(), back_inserter(intersection));
		
		if (intersection.size() != start.tiles.size() || intersection.size() != finish.tiles.size())
			throw ParsingException("States cannot contain different or duplicate tiles");
	}
	
	State::Size parseSize(const string &sizeStr) {
		try {
			State::Size size = tryParsingSize(sizeStr);
			if (size.width < 1 || size.height < 1)
				throw ParsingException("Invalid state size: " + size.toString() + " minimum: <1, 1>");
			
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
		vector<string> parts = split(stateStr, config.tileDelimiter);
		vector<uint8_t> tiles;
		State::Position emptyPos;
		
		bool hasEmpty = false;
		int index = 0, tile;
		for (auto &part : parts) {
			if (part == config.emptyTile) {
				emptyPos.x = index % size.width;
				emptyPos.y = index / size.height;
				
				if (hasEmpty)
					throw ParsingException("Duplicate empty tile at: " + emptyPos.toString());
				else {
					tiles.push_back(0);
					hasEmpty = true;
				}
			} else {
				tile = stoi(part);
				if (0 < tile && tile < 256)
					tiles.push_back(tile);
				else
					throw ParsingException("Invalid tile value: " + to_string(tile) + " valid values are <1,255>");
			}
			
			index++;
		}
		
		if (!hasEmpty)
			throw ParsingException("No empty tile found in: " + stateStr);
	
		return {size, tiles, emptyPos};
	}
	
	ScoreCalculator *parseScoreCalculator(const string &cStr, Heuristics *h) {
		if (cStr == "gh")
			return new GHCalculator(h);
		else if (cStr == "h")
			return new HCalculator(h);
		
		throw ParsingException("Invalid calculator: " + cStr + " valid options are: gh,h");
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
