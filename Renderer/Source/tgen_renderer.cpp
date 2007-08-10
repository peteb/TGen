/*
 *  tgen_renderer.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/10/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "tgen_renderer.h"

bool TGen::isRendererDebug() {
	#ifdef _DEBUG
	return true;
	#else
	return false;
	#endif
}

