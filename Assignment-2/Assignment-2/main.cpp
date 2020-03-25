#include <iostream>
#include "./ArgsParser.hpp"
#include "./Solver.hpp"

using namespace std;

vector<string> toArgs(int argc, const char *argv[]);
void printUsage(const string &programName);
void printResult(const vector<State> result);
void printDirection(const State &state);
void printState(const State &state);

const ArgsParser::Config config = {",", "_"};

int main(int argc, const char *argv[]) {
	vector<string> args = toArgs(argc, argv);
	
	try {
		ArgsParser parser{config};
		ArgsParser::Args parsedArgs = parser.parse(args);
		Solver solver(parsedArgs.scoreCalculator);
		printResult(solver.solve(parsedArgs.start, parsedArgs.finish));
	} catch (const ArgsParser::ParsingException &e) {
		cerr << e.what() << '\n';
		printUsage(args[0]);
	} catch (const Solver::SolvingException &e) {
		cerr << e.what() << '\n';
		printUsage(args[0]);
	}
	
	return 0;
}

vector<string> toArgs(int argc, const char *argv[]) {
	vector<string> args;
	for (int i = 0; i < argc; i++)
		args.push_back(argv[i]);

	return args;
}

void printUsage(const string &programName) {
	cout << "Usage: " << programName << " heuristics calculator dimension start finish\n";
	cout << "\t[heuristics] - Which heuristics to use. Valid options are: 1 - Spot, 2 - Distance, 3 - Sum heuristics\n";
	cout << "\t[calculator] - Determines how the final score of state is calculated. Valid options are: gh, h\n";
	cout << "\t[dimension] - Determines sizes of start and finish states. Example: 3x4\n";
	cout << "\t[start|finish] - List of integers separated by '" + config.tileDelimiter + "' including one '" + config.emptyTile + "' for empty tile\n";
}

void printResult(const vector<State> result) {
	auto it = result.rbegin();
	printState(*it);
	cout << '\n';
	
	for (it++; it != result.rend(); it++) {
		printDirection(*it);
		printState(*it);
		cout << '\n';
	}
}

void printDirection(const State &state) {
	switch (state.parentDirection) {
		case State::Top:
			cout << "Top\n";
			break;
			
		case State::Left:
			cout << "Left\n";
			break;
			
		case State::Right:
			cout << "Right\n";
			break;
			
		case State::Bottom:
			cout << "Bottom\n";
			break;
	}
}

void printState(const State &state) {
	int index = 0;
	for (auto &tile : state.tiles) {
		if (tile == 0)
			cout << config.emptyTile;
		else
			cout << static_cast<int>(tile);
		
		cout << '\t';
		
		if (++index % state.size.width == 0)
			cout << '\n';
	}
}
