#import <XCTest/XCTest.h>
#include "Crossing.hpp"

@interface CrossingTests : XCTestCase

@end

@implementation CrossingTests

-(void)testSliceCrossingChanceZero {
	Instruction i1{0b00000000};
	Instruction i2{0b11111111};
	SliceCrossing g;
	XCTAssertEqual(g.generateInstruction(i1, i2, 0.0), i1);
}

-(void)testSliceCrossingChanceHalf {
	Instruction i1{0b00000000};
	Instruction i2{0b11111111};
	SliceCrossing g;
	XCTAssertEqual(g.generateInstruction(i1, i2, 0.5), 0b00001111);
}

-(void)testSliceCrossingChanceOne {
	Instruction i1{0b00000000};
	Instruction i2{0b11111111};
	SliceCrossing g;
	XCTAssertEqual(g.generateInstruction(i1, i2, 1.0), i2);
}

@end
