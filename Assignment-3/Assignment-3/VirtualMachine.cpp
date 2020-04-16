#include "VirtualMachine.hpp"

VirtualMachine::VirtualMachine(const Map &m, bool stae, int il) :
	map(m),
	shouldTerminateAtEnd(stae),
	instructionLimit(il) {}

VirtualMachine::Result VirtualMachine::execute(const Program &p) {
	prepare(p);
	
	for (instructionIndex = 0; instructionsExecuted < instructionLimit;) {
		executeInstruction();
		instructionsExecuted++;
		
		if (isOutOfBounds())
			return Result{instructionsExecuted, static_cast<int>(collectedTreasures.size()), OutOfBounds, program};
		
		if (isOnTreasure() && treasureWasNotCollected())
			collect();
		
		if (didCollectAllTreasures())
			return Result{instructionsExecuted, static_cast<int>(collectedTreasures.size()), Success, program};
		
		if (didExecuteAllInstructions()) {
			if (shouldTerminateAtEnd)
				return Result{instructionsExecuted, static_cast<int>(collectedTreasures.size()), EndOfProgram, program};
			else
				jumpToStart();
		} else
			instructionIndex++;
	}
	
	return Result{instructionsExecuted, static_cast<int>(collectedTreasures.size()), InstructionLimit, program};
}

void VirtualMachine::prepare(const Program &p) {
	program = p;
	instructionsExecuted = 0;
	collectedTreasures.clear();
	playerPosition = map.start;
}

void VirtualMachine::executeInstruction() {
	Instruction &i = currentInstruction();
	
	switch (i.type()) {
		case Instruction::Increment:
			increment(i.address());
			break;
			
		case Instruction::Decrement:
			decrement(i.address());
			break;
			
		case Instruction::Jump:
			jump(i.address());
			break;
			
		case Instruction::Print:
			print(i.address());
			break;
	}
}

Instruction &VirtualMachine::currentInstruction() {
	return program[instructionIndex];
}

void VirtualMachine::increment(uint8_t address) {
	++program[address];
}

void VirtualMachine::decrement(uint8_t address) {
	--program[address];
}

void VirtualMachine::jump(uint8_t address) {
	instructionIndex = address - 1;
}

void VirtualMachine::print(uint8_t address) {
	switch (program[address].direction()) {
		case Instruction::Direction::Top:
			playerPosition.y++;
			break;
			
		case Instruction::Direction::Left:
			playerPosition.x--;
			break;
			
		case Instruction::Direction::Right:
			playerPosition.x++;
			break;
			
		case Instruction::Direction::Bottom:
			playerPosition.y--;
			break;
	}
}

bool VirtualMachine::isOutOfBounds() {
	return (
		playerPosition.x < 0 ||
		playerPosition.y < 0 ||
		playerPosition.x >= map.size.x ||
		playerPosition.y >= map.size.y
	);
}

bool VirtualMachine::isOnTreasure() {
	return map.isOnTreasure(playerPosition);
}

bool VirtualMachine::treasureWasNotCollected() {
	return collectedTreasures.find(playerPosition) == collectedTreasures.end();
}

void VirtualMachine::collect() {
	collectedTreasures.insert(playerPosition);
}

bool VirtualMachine::didCollectAllTreasures() {
	return map.treasureCount() == collectedTreasures.size();
}

bool VirtualMachine::didExecuteAllInstructions() {
	return instructionIndex == (program.size() - 1);
}

void VirtualMachine::jumpToStart() {
	instructionIndex = 0;
}
