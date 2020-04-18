#ifndef Instruction_hpp
#define Instruction_hpp

#include <cstdint>

using namespace std;

struct Instruction {
	enum Type : uint8_t {
		Increment = 0b00'000000,
		Decrement = 0b01'000000,
		Jump      = 0b10'000000,
		Print     = 0b11'000000,
	};
	
	enum Direction : uint8_t {
		Top    = 0b000000'00,
		Left   = 0b000000'01,
		Right  = 0b000000'10,
		Bottom = 0b000000'11,
	};
	
	Instruction() = default;
	Instruction(uint8_t r) : raw(r) {}
	Instruction(Type t, uint8_t a) : raw(t | (a & 0b00111111)) {}
	
	Instruction operator++() {++raw; return *this;}
	Instruction operator+(int n) {raw += n; return *this;}
	Instruction operator--() {--raw; return *this;}
	Instruction operator-(int n) {raw -= n; return *this;}
	bool operator==(const Instruction& b) const {return raw == b.raw;}
	bool operator==(int n) const {return raw == n;}
	bool operator!=(const Instruction& b) const {return raw != b.raw;}
	
	Type type() const {
		return static_cast<Type>(raw & 0b11'000000);
	}
	
	uint8_t address() const {
		return raw & 0b00'111111;
	}
								 
	 Direction direction() {
		 return static_cast<Direction>(raw & 0b000000'11);
	 }
	
	uint8_t raw;
};

#endif
