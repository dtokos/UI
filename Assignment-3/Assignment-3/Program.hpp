#ifndef Program_hpp
#define Program_hpp

#include <array>
#include "Instruction.hpp"
#include "Random.hpp"

using namespace std;

struct Program {
	array<Instruction, 64> instructions;
	Instruction& operator[](int index) {return instructions[index];}
	const Instruction& operator[](int index) const {return instructions[index];}
	
	static Program random() {
		array<Instruction, 64> instructions;
		RandRandom rnd;
		
		for (auto &instruction : instructions)
			instruction.raw = rnd.to(256);
		
		return Program{instructions};
	}
	
	constexpr size_t size() const {
		return instructions.size();
	}
};

#endif
