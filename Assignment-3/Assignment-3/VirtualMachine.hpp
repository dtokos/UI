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
	
	struct Result {
		int instructionsExecuted;
		int collectedTreasures;
		Termination termination;
		Program program;
	};
	
	VirtualMachine(bool stae, int il);
	Result execute(const Program &program, const Map &map);
	
private:
	bool shouldTerminateAtEnd;
	int instructionLimit;
	
	Map::Vector playerPosition;
	int instructionsExecuted, instructionIndex;
	set<Map::Vector> collectedTreasures;
	Program program;
	const Map *map;
	
	void prepare(const Program &program, const Map &map);
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
