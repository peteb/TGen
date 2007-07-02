/*
 *  tgen_graphics.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/2/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "tgen_graphics.h"

bool TGen::isGraphicsDebug() {
	#ifdef _DEBUG
	return true;
	#else
	return false;
	#endif
}

