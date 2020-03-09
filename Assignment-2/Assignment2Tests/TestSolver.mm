#import <XCTest/XCTest.h>
#include "Solver.hpp"

@interface TestSolver : XCTestCase

@end

@implementation TestSolver

-(void)testDifferentSizesShouldThrow {
	State s1{{5, 10}, {1, 2, 3}, {0, 0}};
	State s2{{15, 20}, {1, 2, 3}, {0, 0}};
	SpotHeuristics h;
	Solver solver{&h};
	XCTAssertThrows(solver.solve(s1, s2));
}

-(void)testUnsolvableShouldThrow {
	State s1{{2, 2}, {1, 2, 3, 4}, {0, 0}};
	State s2{{2, 2}, {1, 3, 2, 4}, {0, 0}};
	SpotHeuristics h;
	Solver solver{&h};
	XCTAssertThrows(solver.solve(s1, s2));
}

-(void)testSolveSimple {
	State s1{{2, 2}, {1, 2, 3, 4}, {0, 0}};
	State s2{{2, 2}, {2, 4, 3, 1}, {1, 1}};
	SpotHeuristics h;
	Solver solver{&h};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)test3x3SolvableExampleWithSpotHeuristics {
	State s1{{3, 3}, {1, 8, 2, -1, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, -1}, {2, 2}};
	SpotHeuristics h;
	Solver solver{&h};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)test3x3UnsolvableExampleWithSpotHeuristics {
	State s1{{3, 3}, {8, 1, 2, -1, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, -1}, {2, 2}};
	SpotHeuristics h;
	Solver solver{&h};
	XCTAssertThrows(solver.solve(s1, s2));
}

-(void)test3x3SolvableExampleWithDistanceHeuristics {
	State s1{{3, 3}, {1, 8, 2, -1, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, -1}, {2, 2}};
	DistanceHeuristics h;
	Solver solver{&h};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)test3x3UnsolvableExampleWithDistanceHeuristics {
	State s1{{3, 3}, {8, 1, 2, -1, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, -1}, {2, 2}};
	DistanceHeuristics h;
	Solver solver{&h};
	XCTAssertThrows(solver.solve(s1, s2));
}

-(void)test3x3SolvableExampleWithSumHeuristics {
	State s1{{3, 3}, {1, 8, 2, -1, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, -1}, {2, 2}};
	SpotHeuristics sH;
	DistanceHeuristics dH;
	SumHeuristics h{{&sH, &dH}};
	Solver solver{&h};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)test3x3UnsolvableExampleWithSumHeuristics {
	State s1{{3, 3}, {8, 1, 2, -1, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, -1}, {2, 2}};
	SpotHeuristics sH;
	DistanceHeuristics dH;
	SumHeuristics h{{&sH, &dH}};
	Solver solver{&h};
	XCTAssertThrows(solver.solve(s1, s2));
}

@end
