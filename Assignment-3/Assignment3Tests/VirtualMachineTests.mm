#import <XCTest/XCTest.h>
#include "VirtualMachine.hpp"

@interface VirtualMachineTests : XCTestCase

@end

@implementation VirtualMachineTests

-(void)testSingleIncrement {
	Program p{{0b00000000}};
	Map m{{1, 1}, {0, 0}, {{1, 1}}};
	VirtualMachine vm{true, 1};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.program[0], p[0] + 1);
}

-(void)testTwoIncrements {
	Program p{{0b00000010, 0b00000010}};
	Map m{{1, 1}, {0, 0}, {{1, 1}}};
	VirtualMachine vm{true, 2};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.program[2], p[2] + 2);
}

-(void)testIncrementOverflow {
	Program p{{0b00000001, 0b11111111, 0b00000000}};
	Map m{{1, 1}, {0, 0}, {{1, 1}}};
	VirtualMachine vm{true, 1};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.program[1], p[2]);
}

-(void)testSingleDecrement {
	Program p{{0b01000000}};
	Map m{{1, 1}, {0, 0}, {{1, 1}}};
	VirtualMachine vm{true, 1};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.program[0], p[0] - 1);
}

-(void)testTwoDecrements {
	Program p{{0b01000010, 0b01000010, 0b00000010}};
	Map m{{1, 1}, {0, 0}, {{1, 1}}};
	VirtualMachine vm{true, 2};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.program[2], p[2] - 2);
}

-(void)testDecrementOverflow {
	Program p{{0b01000001, 0b00000000, 0b11111111}};
	Map m{{1, 1}, {0, 0}, {{1, 1}}};
	VirtualMachine vm{true, 1};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.program[1], p[2]);
}

-(void)testJump {
	Program p{{0b10000010, 0b00000000, 0b00000011, 0b00000000}};
	Map m{{1, 1}, {0, 0}, {{1, 1}}};
	VirtualMachine vm{true, 2};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.program[3], p[3] + 1);
}

-(void)testPrint {
	Program p{{0b11000000}};
	Map m{{2, 2}, {0, 0}, {{0, 1}}};
	VirtualMachine vm{true, 1};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.collectedTreasures, 1);
}

-(void)testTerminationInstructionLimit {
	Program p;
	Map m{{10, 10}, {0, 0}, {{9, 9}}};
	VirtualMachine vm{true, 5};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.termination, VirtualMachine::InstructionLimit);
}

-(void)testTerminationEndOfProgram {
	Program p;
	Map m{{10, 10}, {0, 0}, {{9, 9}}};
	VirtualMachine vm{true, 100};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.termination, VirtualMachine::EndOfProgram);
	XCTAssertEqual(r.instructionsExecuted, p.size());
}

-(void)testProgramLoopingAtEnd {
	Program p;
	Map m{{10, 10}, {0, 0}, {{9, 9}}};
	VirtualMachine vm{false, 100};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.termination, VirtualMachine::InstructionLimit);
	XCTAssertEqual(r.instructionsExecuted, 100);
}

-(void)testTerminationOutOfBoundsTop {
	Program p{{0b11000000, 0b11000000, 0b11000000}};
	Map m{{5, 2}, {0, 0}, {{9, 9}}};
	VirtualMachine vm{true, 100};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.termination, VirtualMachine::OutOfBounds);
	XCTAssertEqual(r.instructionsExecuted, 2);
}

-(void)testTerminationOutOfBoundsLeft {
	Program p{{0b11000001, 0b11000001, 0b11000001}};
	Map m{{2, 5}, {1, 0}, {{9, 9}}};
	VirtualMachine vm{true, 100};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.termination, VirtualMachine::OutOfBounds);
	XCTAssertEqual(r.instructionsExecuted, 2);
}

-(void)testTerminationOutOfBoundsRight {
	Program p{{0b11000010, 0b11000010, 0b11000010}};
	Map m{{2, 5}, {0, 0}, {{9, 9}}};
	VirtualMachine vm{true, 100};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.termination, VirtualMachine::OutOfBounds);
	XCTAssertEqual(r.instructionsExecuted, 2);
}

-(void)testTerminationOutOfBoundsBottom {
	Program p{{0b11000011, 0b11000011, 0b11000011, 0b11000011}};
	Map m{{5, 2}, {0, 1}, {{9, 9}}};
	VirtualMachine vm{true, 100};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.termination, VirtualMachine::OutOfBounds);
	XCTAssertEqual(r.instructionsExecuted, 2);
}

-(void)testTerminationSuccess {
	Program p{{0b11000000, 0b11000001, 0b11000000}};
	Map m{{2, 3}, {1, 0}, {{1, 1}, {0, 1}, {0, 2}}};
	VirtualMachine vm{true, 100};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.termination, VirtualMachine::Success);
	XCTAssertEqual(r.instructionsExecuted, 3);
	XCTAssertEqual(r.collectedTreasures, m.treasureCount());
}

-(void)testPath {
	Program p{{0b11000000, 0b11000001, 0b11000000}};
	Map m{{2, 3}, {1, 0}, {{1, 1}, {0, 1}, {0, 2}}};
	VirtualMachine vm{true, 100};
	auto r = vm.execute<true>(p, m);
	XCTAssertEqual(r.path.size(), 3);
	XCTAssertEqual(r.path[0], Instruction::Direction::Top);
	XCTAssertEqual(r.path[1], Instruction::Direction::Left);
	XCTAssertEqual(r.path[2], Instruction::Direction::Top);
}

@end
