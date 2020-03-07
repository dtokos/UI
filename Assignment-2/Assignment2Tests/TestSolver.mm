#import <XCTest/XCTest.h>
#include "Solver.hpp"

@interface TestSolver : XCTestCase

@end

@implementation TestSolver

-(void)testDifferentSizesShouldThrow {
	State s1{{5, 10}, {1, 2, 3}};
	State s2{{5, 10}, {1, 2, 3}};
	Solver solver;
	XCTAssertThrows(solver.solve(s1, s2));
}

-(void)testUnsolvableShouldThrow {
	State s1{{2, 2}, {1, 2, 3, 4}};
	State s2{{2, 2}, {2, 1, 3, 4}};
	Solver solver;
	XCTAssertThrows(solver.solve(s1, s2));
}

@end
