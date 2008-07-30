/*
 *  testcase_vecmat.h
 *  TGen Math
 *
 *  Created by Peter Backman on 7/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include <CPlusTest/CPlusTest.h>

class VecMatTestCase : public TestCase {
public:
	VecMatTestCase(TestInvocation * invocation);
	virtual ~VecMatTestCase();
	
	void testBasic();
	
private:
	void testBasicVec3();
};
