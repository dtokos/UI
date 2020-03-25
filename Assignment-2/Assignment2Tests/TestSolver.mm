#import <XCTest/XCTest.h>
#include "Solver.hpp"

@interface TestSolver : XCTestCase

@end

@implementation TestSolver

-(void)testDifferentSizesShouldThrow {
	State s1{{5, 10}, {1, 2, 3}, {0, 0}};
	State s2{{15, 20}, {1, 2, 3}, {0, 0}};
	SpotHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertThrows(solver.solve(s1, s2));
}

-(void)testUnsolvableShouldThrow {
	State s1{{2, 2}, {1, 2, 3, 4}, {0, 0}};
	State s2{{2, 2}, {1, 3, 2, 4}, {0, 0}};
	SpotHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertThrows(solver.solve(s1, s2));
}

-(void)testSolveSimple {
	State s1{{2, 2}, {1, 2, 3, 4}, {0, 0}};
	State s2{{2, 2}, {2, 4, 3, 1}, {1, 1}};
	SpotHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)testSimpleResult {
	State s1{{2, 2}, {1, 2, 3, 4}, {0, 0}};
	State s2{{2, 2}, {2, 4, 3, 1}, {1, 1}};
	SpotHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	vector<State> result = solver.solve(s1, s2);
	XCTAssertEqual(3, result.size());
	XCTAssertEqual(Direction::Bottom, result[0].parentDirection);
	XCTAssertEqual(Direction::Right, result[1].parentDirection);
}

-(void)test3x3SolvableExampleWithSpotHeuristics {
	State s1{{3, 3}, {1, 8, 2, 0, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 0}, {2, 2}};
	SpotHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)test3x3SolvableExampleWithSpotHeuristicsResult {
	State s1{{3, 3}, {1, 8, 2, 0, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 0}, {2, 2}};
	SpotHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	vector<State> result = solver.solve(s1, s2);
	XCTAssertEqual(10, result.size());
	XCTAssertEqual(Direction::Bottom, result[0].parentDirection);
	XCTAssertEqual(Direction::Right, result[1].parentDirection);
	XCTAssertEqual(Direction::Top, result[2].parentDirection);
	XCTAssertEqual(Direction::Left, result[3].parentDirection);
	XCTAssertEqual(Direction::Bottom, result[4].parentDirection);
	XCTAssertEqual(Direction::Bottom, result[5].parentDirection);
	XCTAssertEqual(Direction::Right, result[6].parentDirection);
	XCTAssertEqual(Direction::Top, result[7].parentDirection);
	XCTAssertEqual(Direction::Right, result[8].parentDirection);
}

-(void)test3x3UnsolvableExampleWithSpotHeuristics {
	State s1{{3, 3}, {8, 1, 2, 0, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 0}, {2, 2}};
	SpotHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertThrows(solver.solve(s1, s2));
}

-(void)test3x3SolvableExampleWithDistanceHeuristics {
	State s1{{3, 3}, {1, 8, 2, 0, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 0}, {2, 2}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)test3x3UnsolvableExampleWithDistanceHeuristics {
	State s1{{3, 3}, {8, 1, 2, 0, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 0}, {2, 2}};
	DistanceHeuristics h;
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertThrows(solver.solve(s1, s2));
}

-(void)test3x3SolvableExampleWithSumHeuristics {
	State s1{{3, 3}, {1, 8, 2, 0, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 0}, {2, 2}};
	SpotHeuristics sH;
	DistanceHeuristics dH;
	SumHeuristics h{{&sH, &dH}};
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertNoThrow(solver.solve(s1, s2));
}

-(void)test3x3UnsolvableExampleWithSumHeuristics {
	State s1{{3, 3}, {8, 1, 2, 0, 4, 3, 7, 6, 5}, {0, 1}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 0}, {2, 2}};
	SpotHeuristics sH;
	DistanceHeuristics dH;
	SumHeuristics h{{&sH, &dH}};
	GHCalculator c{&h};
	Solver solver{&c};
	XCTAssertThrows(solver.solve(s1, s2));
}

@end
