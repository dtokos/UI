#import <XCTest/XCTest.h>
#include "Solver.hpp"

@interface TestGHDistance : XCTestCase

@end

@implementation TestGHDistance

-(void)testSolve1x1 {
	State s1{{1, 1}, {0}, {0, 0}};
	State s2{{1, 1}, {0}, {0, 0}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)testSolve2x1 {
	State s1{{2, 1}, {0, 1}, {0, 0}};
	State s2{{2, 1}, {1, 0}, {1, 0}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	
}

-(void)testSolve2x2 {
	State s1{{2, 2}, {3, 1, 2, 0}, {1, 1}};
	State s2{{2, 2}, {1, 2, 3, 0}, {1, 1}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)testSolve3x2 {
	State s1{{3, 2}, {1, 3, 5, 0, 4, 2}, {0, 1}};
	State s2{{3, 2}, {1, 2, 3, 4, 5, 0}, {2, 1}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)testSolve3x3 {
	State s1{{3, 3}, {2, 3, 6, 1, 8, 0, 7, 4, 5}, {2, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 0}, {2, 2}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)testSolve4x3 {
	State s1{{4, 3}, {5, 3, 1, 4, 9, 6, 10, 2, 11, 0, 8, 7}, {1, 2}};
	State s2{{4, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0}, {3, 2}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)testSolve4x4 {
	State s1{{4, 4}, {1, 2, 3, 4, 5, 10, 6, 8, 9, 0, 7, 11, 13, 14, 15, 12}, {1, 2}};
	State s2{{4, 4}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}, {3, 3}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)testSolve5x4 {
	State s1{{5, 4}, {1, 7, 14, 12, 4, 11, 2, 8, 3, 5, 17, 15, 18, 9, 19, 6, 0, 16, 13, 10}, {1, 3}};
	State s2{{5, 4}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0}, {4, 3}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)testSolve5x5 {
	State s1{{5, 5}, {2, 8, 7, 0, 13, 11, 18, 9, 5, 4, 6, 3, 14, 15, 10, 1, 22, 17, 12, 19, 16, 21, 23, 24, 20}, {3, 0}};
	State s2{{5, 5}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 0}, {4, 4}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

@end
