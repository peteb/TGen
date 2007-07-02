/*
 *  tgen_core.cpp
 *  TGen Core
 *
 *  Created by Peter Backman on 6/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "tgen_core.h"

bool TGen::isCoreDebug() {
	#ifdef _DEBUG
	return true;
	#else
	return false;
	#endif
}
