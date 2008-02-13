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
#elif defined(_PLATFORM_WIN)
	#include <windows.h>
	#include <GL/GL.h>
	#include <GL/GLU.h>
#else
	#include <GL/GL.h>
	#include <GL/GLU.h>
#endif


#define _OGL_DEBUG

#ifdef _OGL_DEBUG
	#define STAT_ADD(x) stats.addStat(x)
	#define STAT_ADD_VAL(x, val) stats.addStat(x, val)
	#define PSTAT_ADD_VAL(x, val) stats.addPStat(x, val)
	#define STAT_ADD_SHADER(x) stats.addShader(x)
	#define STAT_ADD_TEXTURE(x, y) stats.addTexture(x, y)
#else
	#define STAT_ADD(x)
	#define STAT_ADD_VAL(x, val)
	#define PSTAT_ADD_VAL(x, val)
	#define STAT_ADD_SHADER(x)
	#define STAT_ADD_TEXTURE(x, y)
#endif

#endif // !_TGEN_OPENGL_PREFIX_H
