#import <XCTest/XCTest.h>
#include "Instruction.hpp"

@interface InstructionTests : XCTestCase

@end

@implementation InstructionTests

-(void)testTypeIncrement {
	Instruction i{0b00000000};
	XCTAssertEqual(i.type(), Instruction::Type::Increment);
}

-(void)testTypeDecrement {
	Instruction i{0b01000000};
	XCTAssertEqual(i.type(), Instruction::Type::Decrement);
}

-(void)testTypeJump {
	Instruction i{0b10000000};
	XCTAssertEqual(i.type(), Instruction::Type::Jump);
}

-(void)testTypePrint {
	Instruction i{0b11000000};
	XCTAssertEqual(i.type(), Instruction::Type::Print);
}

-(void)testAddressZero {
	Instruction i{0b11000000};
	XCTAssertEqual(i.address(), 0);
}

-(void)testAddressFive {
	Instruction i{0b11000101};
	XCTAssertEqual(i.address(), 5);
}

-(void)testTypeConstructor {
	Instruction i{Instruction::Type::Decrement, 0};
	XCTAssertEqual(i.type(), Instruction::Type::Decrement);
	XCTAssertEqual(i.address(), 0);
}

-(void)testTypeConstructorAddressOverflow {
	Instruction i{Instruction::Type::Decrement, 0b11111111};
	XCTAssertEqual(i.type(), Instruction::Type::Decrement);
	XCTAssertEqual(i.address(), 0b00111111);
}

-(void)testDirectionTop {
	Instruction i{0b00000000};
	XCTAssertEqual(i.direction(), Instruction::Direction::Top);
}

-(void)testDirectionLeft {
	Instruction i{0b00000001};
	XCTAssertEqual(i.direction(), Instruction::Direction::Left);
}

-(void)testDirectionRight {
	Instruction i{0b00000010};
	XCTAssertEqual(i.direction(), Instruction::Direction::Right);
}

-(void)testDirectionBottom {
	Instruction i{0b00000011};
	XCTAssertEqual(i.direction(), Instruction::Direction::Bottom);
}

-(void)testCompareSameInstructions {
	Instruction i1{0};
	Instruction i2{0};
	XCTAssertTrue(i1 == i2);
}

-(void)testCompareDifferentInstructions {
	Instruction i1{0};
	Instruction i2{1};
	XCTAssertTrue(i1 != i2);
}
	
@end
