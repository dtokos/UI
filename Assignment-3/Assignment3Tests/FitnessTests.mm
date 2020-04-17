#import <XCTest/XCTest.h>
#include "Fitness.hpp"

@interface FitnessTests : XCTestCase

@end

@implementation FitnessTests

-(void)testTreasureDistance {
	TreasureDistanceFitness f{0.001};
	VirtualMachine::Result r{5, 10};
	XCTAssertEqualWithAccuracy(f.calculate(r), 1.0 + 10 - 5 * 0.001, 0.001);
}

-(void)testTreasureDistanceStepWeight {
	TreasureDistanceFitness f{1};
	VirtualMachine::Result r{5, 10};
	XCTAssertEqualWithAccuracy(f.calculate(r), 1.0 + 10 - 5 * 1, 0.001);
}

-(void)testTreasureDistanceNoSteps {
	TreasureDistanceFitness f{0.001};
	VirtualMachine::Result r{0, 0};
	XCTAssertEqualWithAccuracy(f.calculate(r), 1.0, 0.001);
}

@end
