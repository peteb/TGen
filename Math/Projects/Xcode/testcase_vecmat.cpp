/*
 *  testcase_vecmat.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 7/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "testcase_vecmat.h"

#include "tgen_math.h"

VecMatTestCase test1(TEST_INVOCATION(VecMatTestCase, testBasic));

VecMatTestCase::VecMatTestCase(TestInvocation *invocation)
    : TestCase(invocation)
{
	
}


VecMatTestCase::~VecMatTestCase() {
	
}

void VecMatTestCase::testBasic() {
	testBasicVec3();
}

void VecMatTestCase::testBasicVec3() {
	TGen::Vector3 vector1(4.0f, 3.0f, 2.0f);
	TGen::Vector3 vector2(2.0f, 3.0f, 4.0f);
	TGen::Vector3 vector4(1.0f, 0.0f, 0.0f);
	TGen::Vector3 vector5(0.0f, 0.0f, 1.0f);
	TGen::Vector3 vector6(vector1);
	
	CPTAssert(vector6 == vector1);
	CPTAssert(vector1.x == 4.0f);
	CPTAssert(vector1.y == 3.0f);
	CPTAssert(vector1.z == 2.0f);
	CPTAssert(vector1 + vector2 == TGen::Vector3(6.0f, 6.0f, 6.0f));
	CPTAssert(vector1 * 2.0f == TGen::Vector3(8.0f, 6.0f, 4.0f));
	CPTAssert(vector1 * vector2 == TGen::Vector3(8.0f, 9.0f, 8.0f));
	CPTAssert(vector1.getNormalized().getMagnitude() == 1.0f);
	CPTAssert(vector2.getNormalized().getMagnitude() == 1.0f);
	CPTAssert(TGen::Vector3::DotProduct(vector1, vector2) == 4.0f * 2.0f + 3.0f * 3.0f + 2.0f * 4.0f);
	CPTAssert(TGen::Vector3::CrossProduct(vector4, vector5) == TGen::Vector3(0.0f, -1.0f, 0.0f));
	CPTAssert(TGen::Vector3::CrossProduct(vector1, vector2) == TGen::Vector3(6.0f, -12.0f, 6.0f));
	CPTAssert(TGen::Vector3::CrossProduct(-vector1, vector2) == TGen::Vector3(-6.0f, 12.0f, -6.0f));
	CPTAssert(TGen::Vector3::Parse("4 2 -88") == TGen::Vector3(4.0f, 2.0f, -88.0f));
	CPTAssert(TGen::Vector3::Zero == TGen::Vector3(0.0f, 0.0f, 0.0f));
	CPTAssert(TGen::Vector3::Identity == TGen::Vector3(1.0f, 1.0f, 1.0f));
	CPTAssert(TGen::Vector3::Parse("identity") == TGen::Vector3::Identity);
	CPTAssert(TGen::Vector3::Parse("zero") == TGen::Vector3::Zero);
	CPTAssert(std::string(vector2) == "(2, 3, 4)");	
}

