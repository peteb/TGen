/*
 *  tgen_opengl.cpp
 *  TGen OpenGL
 *
 *  Created by Peter Backman on 7/2/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "tgen_opengl.h"

bool TGen::isOpenGLDebug() {
	#ifdef _DEBUG
	return true;
	#else
	return false;
	#endif
}
