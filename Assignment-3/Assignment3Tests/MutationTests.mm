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

-(void)testInvertMutateAll {
	Agent a{Program::random()};
	vector<Agent> p{a};
	InvertMutation m;
	m.mutate(p);
	bool allSame = true;
	
	for (int i = 0; i < a.program.size(); i++) {
		if (p[0].program[i] != a.program[i]) {
			allSame = false;
			break;
		}
	}
	
	XCTAssertFalse(allSame);
}

@end
