#ifndef Program_hpp
#define Program_hpp

#include "Instruction.hpp"
#include <array>

using namespace std;

struct Program {
	array<Instruction, 64> instructions;
	Instruction& operator[](int index) {return instructions[index];}
	const Instruction& operator[](int index) const {return instructions[index];}
	
	static Program random() {
		array<Instruction, 64> instructions;
		for (auto &instruction : instructions)
			instruction.raw = rand() % 256;
		
		return Program{instructions};
	}
	
	constexpr size_t size() const {
		return instructions.size();
	}
};

#endif
