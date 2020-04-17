#import <XCTest/XCTest.h>
#include "Mutation.hpp"

@interface MutationTests : XCTestCase

@end

@implementation MutationTests

-(void)testInvertMutateChanceZero {
	Instruction i{0};
	InvertMutation m;
	m.mutateInstruction(i, 0.0);
	XCTAssertEqual(i, 0b00000001);
}

-(void)testInvertMutateChanceHalf {
	Instruction i{0};
	InvertMutation m;
	m.mutateInstruction(i, 0.5);
	XCTAssertEqual(i, 0b00001000);
}

-(void)testInvertMutateChanceOne {
	Instruction i{0};
	InvertMutation m;
	m.mutateInstruction(i, 1.0);
	XCTAssertEqual(i, 0b10000000);
}

@end
