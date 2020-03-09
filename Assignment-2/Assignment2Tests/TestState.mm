#import <XCTest/XCTest.h>
#include "State.hpp"

@interface TestState : XCTestCase

@end

@implementation TestState

-(void)testSizeEquality {
	State::Size s1{5, 10};
	State::Size s2{5, 10};
	State::Size s3{15, 20};
	XCTAssertTrue(s1 == s2);
	XCTAssertFalse(s1 == s3);
}

-(void)testSizeToString {
	State::Size s1{5, 10};
	XCTAssertEqual("(5, 10)", s1.toString());
}

-(void)testPositionToIndex {
	State::Size s1{5, 10};
	State::Position pos{3, 4};
	XCTAssertEqual(23, pos.toIndex(s1));
}

-(void)testPositionEquality {
	State::Position p1{3, 4};
	State::Position p2{3, 4};
	State::Position p3{7, 8};
	XCTAssertTrue(p1 == p2);
	XCTAssertFalse(p1 == p3);
}

-(void)testStateEquality {
	State s1{{5, 10}, {1, 2, 3}, {0, 0}};
	State s2{{5, 10}, {1, 2, 3}, {0, 0}};
	State s3{{15, 20}, {1, 2, 3}, {0, 0}};
	State s4{{5, 10}, {5, 6, 7}, {0, 0}};
	XCTAssertTrue(s1 == s2);
	XCTAssertFalse(s1 == s3);
	XCTAssertFalse(s1 == s4);
}

-(void)testStateID {
	State s1{{3, 1}, {1, 2, 3}, {0, 0}};
	XCTAssertEqual("1,2,3,", s1.getID());
}

-(void)testStateNeighbourAtEdges {
	State s1{{2, 2}, {1, 2, 3, 4}, {0, 0}};
	State s2{{2, 2}, {1, 2, 3, 4}, {1, 1}};
	optional<State> n1 = s1.getNeighbour(State::Direction::Top);
	optional<State> n2 = s1.getNeighbour(State::Direction::Left);
	optional<State> n3 = s2.getNeighbour(State::Direction::Right);
	optional<State> n4 = s2.getNeighbour(State::Direction::Bottom);
	XCTAssertEqual(nullopt, n1);
	XCTAssertEqual(nullopt, n2);
	XCTAssertEqual(nullopt, n3);
	XCTAssertEqual(nullopt, n4);
}

-(void)testStateNeighbours {
	State s1{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 1}};
	optional<State> n1 = s1.getNeighbour(State::Direction::Top);
	optional<State> n2 = s1.getNeighbour(State::Direction::Left);
	optional<State> n3 = s1.getNeighbour(State::Direction::Right);
	optional<State> n4 = s1.getNeighbour(State::Direction::Bottom);
	XCTAssertEqual("1,5,3,4,2,6,7,8,9,", n1->getID());
	XCTAssertEqual("1,2,3,5,4,6,7,8,9,", n2->getID());
	XCTAssertEqual("1,2,3,4,6,5,7,8,9,", n3->getID());
	XCTAssertEqual("1,2,3,4,8,6,7,5,9,", n4->getID());
}

@end
