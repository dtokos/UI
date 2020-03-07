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


-(void)testStateEquality {
	State s1{{5, 10}, {1, 2, 3}};
	State s2{{5, 10}, {1, 2, 3}};
	State s3{{15, 20}, {1, 2, 3}};
	State s4{{5, 10}, {5, 6, 7}};
	XCTAssertTrue(s1 == s2);
	XCTAssertFalse(s1 == s3);
	XCTAssertFalse(s1 == s4);
}

@end
