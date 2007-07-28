/*
 *  prefix_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_PREFIX_H
#define _TGEN_OPENGL_PREFIX_H

#include "platform.h"

#ifdef _PLATFORM_OSX
	#include <OpenGL/OpenGL.h>
	#include <GLUT/GLUT.h>
#else
	#include <windows.h>
	#include <GL/GL.h>
#endif

#include "binder_ogl.h"

#endif // !_TGEN_OPENGL_PREFIX_H
