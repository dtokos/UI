#include <iostream>
#include <iomanip>
#include <bitset>
#include "Evolution.hpp"
#include "ArgsParser.hpp"

using namespace std;

void printResult(const Evolution::Result &result, const Map &map);
void printPath(const vector<Instruction::Direction> &path, const Map &map);
void printProgram(const Program &p);
int indexWidth(int programLength);
void printInstructionType(const Instruction &i);

int main(int argc, const char * argv[]) {
	try {
		ArgsParser parser;
		ArgsParser::Args args = parser.parse(argc, argv);
		Evolution e{args.config};
		
		printResult(e.start(args.map, args.populationSize, args.populationLimit), args.map);
		
		return EXIT_SUCCESS;
	} catch (const ArgsParser::ParsingError &e) {
		cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
}

void printResult(const Evolution::Result &r, const Map &m) {
	cout << "Result: " << (r.success ? "success" : "reached population limit") << '\n'
		<< "Population: " << r.populationNumber << '\n'
		<< "Path: \n";
	printPath(r.path, m);
	cout << "Program:\n";
	printProgram(r.program);
}

void printPath(const vector<Instruction::Direction> &path, const Map &m) {
	Map::Vector p{m.start.x + 1, m.start.y + 1};
	cout << "  Start  (" << p.x << ", " << p.y << ")\n";
	for (const auto &d : path) {
		switch (d) {
			case Instruction::Direction::Top:
				cout << "  Up     ";
				p.y++;
				break;
				
			case Instruction::Direction::Left:
				cout << "  Left   ";
				p.x--;
				break;
				
			case Instruction::Direction::Right:
				cout << "  Right  ";
				p.x++;
				break;
				
			case Instruction::Direction::Bottom:
				cout << "  Bottom ";
				p.y--;
				break;
		}
		
		cout << "(" << p.x << ", " << p.y << ")\n";
	}
}

void printProgram(const Program &p) {
	const int iw = indexWidth(static_cast<int>(p.size()));
	for (int i = 0; i < p.size(); i++) {
		cout << "  [" << setw(iw) << i << "]  0b" << bitset<8>(p[i].raw) << "    # ";
		printInstructionType(p[i]);
		cout << ' ' << static_cast<int>(p[i].address()) << '\n';
	}
}

int indexWidth(int programLength) {
	int width = 0;
	for (; programLength > 0; programLength /= 10, width++)
		;
	return width;
}

void printInstructionType(const Instruction &i) {
	switch (i.type()) {
		case Instruction::Increment:
			cout << "Increment";
			break;
			
		case Instruction::Decrement:
			cout << "Decrement";
			break;
			
		case Instruction::Jump:
			cout << "Jump     ";
			break;
			
		case Instruction::Print:
			cout << "Print    ";
			break;
	}
}
