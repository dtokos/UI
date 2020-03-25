#import <XCTest/XCTest.h>
#include "ScoreCalculator.hpp"

@interface TestScoreCalculator : XCTestCase

@end

@implementation TestScoreCalculator

-(void)testGHCalculator {
	SpotHeuristics h;
	GHCalculator c(&h);
	State s1{{3, 1}, {1, 2, 3}, {0, 0}};
	State s2{{3, 1}, {1, 2, 3}, {0, 0}};
	State s3{{3, 1}, {3, 1, 2}, {0, 0}};
	State s4{{3, 1}, {1, 3, 2}, {0, 0}};
	h.setFinal(&s2);
	c.calculate(&s1);
	XCTAssertEqual(s1.score.f, 0);
	XCTAssertEqual(s1.score.g, 0);
	XCTAssertEqual(s1.score.h, 0);
	s1.score.g = 1;
	h.setFinal(&s3);
	c.calculate(&s1);
	XCTAssertEqual(s1.score.f, 4);
	XCTAssertEqual(s1.score.g, 1);
	XCTAssertEqual(s1.score.h, 3);
	h.setFinal(&s4);
	c.calculate(&s1);
	XCTAssertEqual(s1.score.f, 3);
	XCTAssertEqual(s1.score.g, 1);
	XCTAssertEqual(s1.score.h, 2);
}

-(void)testHCalculator {
	SpotHeuristics h;
	HCalculator c(&h);
	State s1{{3, 1}, {1, 2, 3}, {0, 0}};
	State s2{{3, 1}, {1, 2, 3}, {0, 0}};
	State s3{{3, 1}, {3, 1, 2}, {0, 0}};
	State s4{{3, 1}, {1, 3, 2}, {0, 0}};
	h.setFinal(&s2);
	c.calculate(&s1);
	XCTAssertEqual(s1.score.f, 0);
	XCTAssertEqual(s1.score.g, 0);
	XCTAssertEqual(s1.score.h, 0);
	s1.score.g = 1;
	h.setFinal(&s3);
	c.calculate(&s1);
	XCTAssertEqual(s1.score.f, 3);
	XCTAssertEqual(s1.score.g, 1);
	XCTAssertEqual(s1.score.h, 3);
	h.setFinal(&s4);
	c.calculate(&s1);
	XCTAssertEqual(s1.score.f, 2);
	XCTAssertEqual(s1.score.g, 1);
	XCTAssertEqual(s1.score.h, 2);
}

@end
