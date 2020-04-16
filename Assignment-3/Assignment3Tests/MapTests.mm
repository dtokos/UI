#import <XCTest/XCTest.h>
#include "Map.hpp"

@interface MapTests : XCTestCase

@end

@implementation MapTests

-(void)testCompareSameVectors {
	Map::Vector v1{0, 0};
	Map::Vector v2{0, 0};
	XCTAssertTrue(v1 == v2);
}

-(void)testCompareDifferentVectors {
	Map::Vector v1{0, 0};
	Map::Vector v2{0, 1};
	XCTAssertTrue(v1 != v2);
}

-(void)testPositionThatIsOnTreasure {
	Map m{{1, 1}, {0, 0}, {{0, 0}}};
	Map::Vector p{0, 0};
	XCTAssertTrue(m.isOnTreasure(p));
}

-(void)testPositionThatIsNotOnTreasure {
	Map m{{1, 1}, {0, 0}, {{0, 0}}};
	Map::Vector p{0, 1};
	XCTAssertFalse(m.isOnTreasure(p));
}

@end
