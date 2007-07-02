/*
 *  tgen_math.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 6/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "tgen_math.h"

bool TGen::isMathDebug() {
	#ifdef _DEBUG
	return true;
	#else
	return false;
	#endif
}
