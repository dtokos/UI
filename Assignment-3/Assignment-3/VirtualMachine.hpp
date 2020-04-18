#ifndef VirtualMachine_hpp
#define VirtualMachine_hpp

#include <set>
#include "Map.hpp"
#include "Program.hpp"

using namespace std;

class VirtualMachine {
public:
	enum Termination {
		InstructionLimit,
		EndOfProgram,
		OutOfBounds,
		Success,
	};
	
	template <bool>
	struct Result {};
	
	template <>
	struct Result<true> {
		int instructionsExecuted;
		int collectedTreasures;
		Termination termination;
		vector<Instruction::Direction> path;
		Program program;
	};
	
	template<>
	struct Result<false> {
		int instructionsExecuted;
		int collectedTreasures;
		Termination termination;
		vector<Instruction::Direction> path;
	};
	
	VirtualMachine(bool stae, int il);
	template <bool B>
	Result<B> execute(const Program &program, const Map &map);
	
private:
	bool shouldTerminateAtEnd;
	int instructionLimit;
	
	Map::Vector playerPosition;
	int instructionsExecuted, instructionIndex;
	set<Map::Vector> collectedTreasures;
	vector<Instruction::Direction> path;
	Program program;
	const Map *map;
	
	void prepare(const Program &program, const Map &map);
	template <bool B>
	Result<B> result(const Termination &t);
	void executeInstruction();
	Instruction &currentInstruction();
	
	void increment(uint8_t address);
	void decrement(uint8_t address);
	void jump(uint8_t address);
	void print(uint8_t address);
	
	bool isOutOfBounds();
	bool isOnTreasure() const;
	bool treasureWasNotCollected();
	void collect();
	bool didCollectAllTreasures();
	bool didExecuteAllInstructions();
	void jumpToStart();
};

#endif
