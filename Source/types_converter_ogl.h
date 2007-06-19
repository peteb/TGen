/*
 *  types_converter_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_TYPES_CONVERTER_H
#define _TGEN_OPENGL_TYPES_CONVERTER_H

#include "types.h"
#include "renderer_types.h"

namespace TGen {
	namespace OpenGL {
		uint TgenFormatToOpenGL(TGen::FormatType format);
		uint TgenPrimitiveToOpenGL(TGen::PrimitiveType primitive);
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_TYPES_CONVERTER_H
