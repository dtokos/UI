#import <XCTest/XCTest.h>
#include <array>
#include "ArgsParser.hpp"

@interface ArgsParserTests : XCTestCase

@end

@implementation ArgsParserTests

-(void)testParseMapSizeTooSmall {
	std::array<const char *, 4> argv{"program", "0,0", "0x0", "0,0"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseMapSizeOK {
	std::array<const char *, 4> argv{"program", "3,3", "5x5", "3,3"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseStartBelowMap {
	std::array<const char *, 4> argv{"program", "0,0", "5x5", "1,1"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseStartAboveMap {
	std::array<const char *, 4> argv{"program", "6,6", "5x5", "1,1"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseStartBottomLeft {
	std::array<const char *, 4> argv{"program", "1,1", "5x5", "1,1"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseStartTopRight {
	std::array<const char *, 4> argv{"program", "5,5", "5x5", "1,1"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseTreasureBelowMap {
	std::array<const char *, 4> argv{"program", "3,3", "5x5", "0,0"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseTreasureAboveMap {
	std::array<const char *, 4> argv{"program", "3,3", "5x5", "6,6"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseTreasureBottomLeft {
	std::array<const char *, 4> argv{"program", "3,3", "5x5", "1,1"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseTreasureTopRight {
	std::array<const char *, 4> argv{"program", "3,3", "5x5", "5,5"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_rnd_seed_BelowRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--rnd_seed=-1"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_rnd_seed_InRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--rnd_seed=0"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_tdf_sw_BelowRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--tdf_sw=-0.001"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_tdf_sw_AboveRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--tdf_sw=1.001"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_tdf_sw_InRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--tdf_sw=0.5"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_im_limit_BelowRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--im_limit=-0.001"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_im_limit_AboveRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--im_limit=1.001"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_im_limit_InRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--im_limit=0.5"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_vm_stae_Invalid {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--vm_stae=foo"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_vm_stae_Valid {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--vm_stae=true"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_vm_il_BelowRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--vm_il=0"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_vm_il_InRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--vm_il=1"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_ev_pl_BelowRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--ev_pl=0"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_ev_pl_InRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--ev_pl=1"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_ev_ps_BelowRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--ev_ps=0"};
	ArgsParser p;
	XCTAssertThrows(p.parse(argv.size(), argv.data()));
}

-(void)testParseArg_ev_ps_InRange {
	std::array<const char *, 5> argv{"program", "3,3", "5x5", "3,3", "--ev_ps=1"};
	ArgsParser p;
	XCTAssertNoThrow(p.parse(argv.size(), argv.data()));
}

@end
