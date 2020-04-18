#import <XCTest/XCTest.h>
#include <algorithm>
#include <array>
#include "Random.hpp"

@interface RandomTests : XCTestCase

@end

@implementation RandomTests

-(void)testRandRandomToRange {
	std::array<int, 100> a;
	RandRandom r;
	std::generate(a.begin(), a.end(), [r]() mutable {return r.to(100);});
	for (const auto &n : a)
		XCTAssertTrue(0 <= n && n < 100);
}

-(void)testRandRandomToEquality {
	std::array<int, 100> a;
	RandRandom r;
	std::generate(a.begin(), a.end(), [r]() mutable {return r.to(100);});
	bool allSame = true;
	
	for (int i = 0; i < a.size() - 1; i++) {
		if (a[i] != a[i] + 1) {
			allSame = false;
			break;
		}
	}
	
	XCTAssertFalse(allSame);
}

-(void)testRandRandomChanceRange {
	std::array<float, 100> a;
	RandRandom r;
	std::generate(a.begin(), a.end(), [r]() mutable {return r.chance();});
	for (const auto &n : a)
		XCTAssertTrue(0.0 <= n && n <= 1.0);
}

-(void)testRandRandomChanceEquality {
	std::array<float, 100> a;
	RandRandom r;
	std::generate(a.begin(), a.end(), [r]() mutable {return r.chance();});
	bool allSame = true;
	
	for (int i = 0; i < a.size() - 1; i++) {
		if (a[i] != a[i] + 1) {
			allSame = false;
			break;
		}
	}
	
	XCTAssertFalse(allSame);
}

@end
