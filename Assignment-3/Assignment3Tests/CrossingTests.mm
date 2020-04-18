#import <XCTest/XCTest.h>
#include "Crossing.hpp"

@interface CrossingTests : XCTestCase

@end

@implementation CrossingTests

-(void)testSliceCrossingChanceZero {
	Instruction i1{0b00000000};
	Instruction i2{0b11111111};
	RandRandom rnd;
	SliceCrossing g{&rnd};
	XCTAssertEqual(g.generateInstruction(i1, i2, 0.0), i1);
}

-(void)testSliceCrossingChanceHalf {
	Instruction i1{0b00000000};
	Instruction i2{0b11111111};
	RandRandom rnd;
	SliceCrossing g{&rnd};
	XCTAssertEqual(g.generateInstruction(i1, i2, 0.5), 0b00001111);
}

-(void)testSliceCrossingChanceOne {
	Instruction i1{0b00000000};
	Instruction i2{0b11111111};
	RandRandom rnd;
	SliceCrossing g{&rnd};
	XCTAssertEqual(g.generateInstruction(i1, i2, 1.0), i2);
}

-(void)testSliceCrossingGenerate {
	Agent a1{Program::random()};
	Agent a2{Program::random()};
	RandRandom rnd;
	SliceCrossing g{&rnd};
	Agent r = g.generate(a1, a2);
	bool allSame = true;
	
	for (int i = 0; i < a1.program.size(); i++) {
		if (r.program[i] != a1.program[i] && r.program[i] != a1.program[i]) {
			allSame = false;
			break;
		}
	}
	
	XCTAssertFalse(allSame);
}

@end
