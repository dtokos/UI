#import <XCTest/XCTest.h>
#include "Program.hpp"

@interface ProgramTests : XCTestCase

@end

@implementation ProgramTests

-(void)testDefault {
	Program p;
	for (const auto& i : p.instructions)
		XCTAssertEqual(i, 0);
}

-(void)testRandom {
	Program def;
	Program rnd = Program::random();
	bool allSame = true;
	
	for (int i = 0; i < def.size(); i++) {
		if (def[i] != rnd[i]) {
			allSame = false;
			break;
		}
	}
	
	XCTAssertFalse(allSame);
}

@end
