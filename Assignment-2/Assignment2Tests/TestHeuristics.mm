#import <XCTest/XCTest.h>
#include "Heuristics.hpp"

@interface TestHeuristics : XCTestCase

@end

@implementation TestHeuristics

-(void)testSpotHeuristics {
	SpotHeuristics spot;
	State s1{{3, 1}, {1, 2, 3}, {0, 0}};
	State s2{{3, 1}, {1, 2, 3}, {0, 0}};
	State s3{{3, 1}, {3, 1, 2}, {0, 0}};
	State s4{{3, 1}, {1, 3, 2}, {0, 0}};
	spot.setFinal(&s2);
	XCTAssertEqual(0, spot.evaluate(&s1));
	spot.setFinal(&s3);
	XCTAssertEqual(3, spot.evaluate(&s1));
	spot.setFinal(&s4);
	XCTAssertEqual(2, spot.evaluate(&s1));
}

-(void)testDistanceHeuristics {
	DistanceHeuristics distance;
	State s1{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9}, {0, 0}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9}, {0, 0}};
	State s3{{3, 3}, {7, 8, 6, 5, 4, 3, 2, 9, 1}, {0, 0}};
	distance.setFinal(&s2);
	XCTAssertEqual(0, distance.evaluate(&s1));
	distance.setFinal(&s3);
	XCTAssertEqual(16, distance.evaluate(&s1));
}

-(void)testSumHeuristics {
	SpotHeuristics spot;
	DistanceHeuristics distance;
	SumHeuristics sum{{&spot, &distance}};
	State s1{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9}, {0, 0}};
	State s2{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9}, {0, 0}};
	State s3{{3, 3}, {7, 8, 6, 5, 4, 3, 2, 9, 1}, {0, 0}};
	sum.setFinal(&s2);
	XCTAssertEqual(0, sum.evaluate(&s1));
	sum.setFinal(&s3);
	XCTAssertEqual(25, sum.evaluate(&s1));
}

@end
