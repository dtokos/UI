#import <XCTest/XCTest.h>
#include "Solver.hpp"

@interface TestSpotSolve : XCTestCase

@end

@implementation TestSpotSolve

- (void)testSpotSolve3x3Performance {
	State s1{{3, 3}, {3, 5, 4, 8, -1, 2, 1, 6, 7}, {1, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, -1}, {2, 2}};
	SpotHeuristics h;
	Solver solver{&h};
	solver.solve(s1, s2);
}

- (void)testSpotSolve5x5Performance {
	State s1{{5, 5}, {10, 5, 7, 2, 4, 16, 1, 6, 3, 9, 15, 12, 11, 8, 14, 21, 22, 13, 19, -1, 18, 20, 17, 23, 24}, {4, 3}};
	State s2{{5, 5}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, -1}, {4, 4}};
	SpotHeuristics h;
	DistanceHeuristics h2;
	SumHeuristics h3{{&h, &h2}};
	Solver solver{&h3};
	solver.solve(s1, s2);
}

@end
