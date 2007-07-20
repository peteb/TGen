/*
 *  tgen_opengl.h
 *  TGen OpenGL
 *
 *  Created by Peter Backman on 7/2/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_H
#define _TGEN_OPENGL_H

#include "tgen_core.h"
#include "tgen_math.h"
#include "tgen_graphics.h"

#include "renderer_ogl.h"
#include "vertexbuffer_ogl.h"
#include "indexbuffer_ogl.h"
#include "framebuffer_ogl.h"
#include "shader_ogl.h"
#include "shaderprogram_ogl.h"
#include "shadervariable_ogl.h"
#include "texture_ogl.h"
#include "types_converter_ogl.h"


namespace TGen {
	bool isOpenGLDebug();
}

#endif // !_TGEN_OPENGL_H
