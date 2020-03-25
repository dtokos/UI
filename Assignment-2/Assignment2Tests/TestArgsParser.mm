#import <XCTest/XCTest.h>
#import "ArgsParser.hpp"

const ArgsParser::Config config = {",", "_", -1};

@interface TestArgsParser : XCTestCase

@end

@implementation TestArgsParser

-(void)testNotEnoughArgumentsShouldThrow {
	ArgsParser parser{config};
	XCTAssertThrows(parser.parse({"program", "1", "2x2", "1,2,3,4"}));
}

-(void)testInvalidSizeShouldThrow {
	ArgsParser parser{config};
	XCTAssertThrows(parser.parse({"program", "1", "gh", "invalid", "1,2,3,_", "1,2,3,_"}));
	XCTAssertThrows(parser.parse({"program", "1", "gh", "3xa", "1,2,3,_", "1,2,3,_"}));
	XCTAssertThrows(parser.parse({"program", "1", "gh", "3", "1,2,3,_", "1,2,3,_"}));
	XCTAssertThrows(parser.parse({"program", "1", "gh", "axa", "1,2,3,_", "1,2,3,_"}));
	XCTAssertThrows(parser.parse({"program", "1", "gh", "0x0", "1,2,3,_", "1,2,3,_"}));
}

-(void)testInvalidStateShouldThrow {
	ArgsParser parser{config};
	XCTAssertThrows(parser.parse({"program", "1", "gh", "2x2", "_,_", "1,2,3,_"}));
	XCTAssertThrows(parser.parse({"program", "1", "gh", "2x2", "1,2", "1,2,3,_"}));
	XCTAssertThrows(parser.parse({"program", "1", "gh", "2x2", "a,b", "1,2,3,_"}));
	XCTAssertThrows(parser.parse({"program", "1", "gh", "2x2", "1,_", "1,2,3,_"}));
}

-(void)testInvalidHeuristicsShouldThrow {
	ArgsParser parser{config};
	XCTAssertThrows(parser.parse({"program", "a", "h", "2x2", "1,2,3,_", "1,2,3,_"}));
	XCTAssertThrows(parser.parse({"program", "4", "h", "2x2", "1,2,3_", "1,2,3,_"}));
}

-(void)testInvalidCalculatorShouldThrow {
	ArgsParser parser{config};
	XCTAssertThrows(parser.parse({"program", "1", "1", "2x2", "1,2,3,_", "1,2,3,_"}));
	XCTAssertThrows(parser.parse({"program", "1", "qw", "2x2", "1,2,3_", "1,2,3,_"}));
}

-(void)testShouldParseValidArguments {
	ArgsParser parser{config};
	ArgsParser::Args args = parser.parse({"program", "1", "gh", "2x2", "1,2,3,_", "1,_,3,2"});
	XCTAssertEqual(2, args.start.size.width);
	XCTAssertEqual(2, args.start.size.height);
	XCTAssertEqual(4, args.start.tiles.size());
	XCTAssertEqual(config.emptyTileValue, args.start.tiles[3]);
	XCTAssertEqual(2, args.finish.size.width);
	XCTAssertEqual(2, args.finish.size.height);
	XCTAssertEqual(4, args.finish.tiles.size());
	XCTAssertEqual(config.emptyTileValue, args.finish.tiles[1]);
	XCTAssertTrue(args.scoreCalculator != NULL);
}

-(void)testShouldParseValidArgumentsWithConfig {
	const ArgsParser::Config c = {"|", " ", -5};
	ArgsParser parser{c};
	ArgsParser::Args args = parser.parse({"program", "1", "gh", "2x2", "1|2|3| ", "1| |3|2"});
	XCTAssertEqual(2, args.start.size.width);
	XCTAssertEqual(2, args.start.size.height);
	XCTAssertEqual(4, args.start.tiles.size());
	XCTAssertEqual(c.emptyTileValue, args.start.tiles[3]);
	XCTAssertEqual(2, args.finish.size.width);
	XCTAssertEqual(2, args.finish.size.height);
	XCTAssertEqual(4, args.finish.tiles.size());
	XCTAssertEqual(c.emptyTileValue, args.finish.tiles[1]);
	XCTAssertTrue(args.scoreCalculator != NULL);
}

@end
