#import <XCTest/XCTest.h>
#include "Selection.hpp"

@interface SelectionTests : XCTestCase

@end

@implementation SelectionTests

-(void)testRouletteChanceZero {
	Agent a1{{}, 1};
	Agent a2{{}, 2};
	vector<Agent> p{a1, a2};
	RouletteSelection s;
	s.prepare(p);
	XCTAssertEqual(s.pick(p, 0.0).fitness, a1.fitness);
}

-(void)testRouletteChanceHalf {
	Agent a1{{}, 1};
	Agent a2{{}, 5};
	Agent a3{{}, 1};
	vector<Agent> p{a1, a2, a3};
	RouletteSelection s;
	s.prepare(p);
	XCTAssertEqual(s.pick(p, 0.5).fitness, a2.fitness);
}

-(void)testRouletteChanceOne {
	Agent a1{{}, 1};
	Agent a2{{}, 2};
	vector<Agent> p{a1, a2};
	RouletteSelection s;
	s.prepare(p);
	XCTAssertEqual(s.pick(p, 1.0).fitness, a2.fitness);
}

-(void)testRouletteBigStart {
	Agent a1{{}, 10};
	Agent a2{{}, 1};
	vector<Agent> p{a1, a2};
	RouletteSelection s;
	s.prepare(p);
	XCTAssertEqual(s.pick(p, 0.2).fitness, a1.fitness);
}


@end
