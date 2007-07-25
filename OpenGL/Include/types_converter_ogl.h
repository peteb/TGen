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
#include "image.h"

namespace TGen {
	namespace OpenGL {
		uint TgenFormatToOpenGL(TGen::FormatType format);
		uint TgenPrimitiveToOpenGL(TGen::PrimitiveType primitive);
		uint TgenImageFormatToOpenGL(TGen::ImageFormat format);
		uint TgenBlendFuncToOpenGL(TGen::BlendFunc blend);
		uint TgenTextureWrapToOpenGL(TGen::TextureWrap wrap);
		uint TgenTextureFilterToOpenGL(TGen::TextureFilter filter);
		uint TgenTextureCoordGenToOpenGL(TGen::TextureCoordGen tcgen);
		uint TgenCompareFuncToOpenGL(TGen::CompareFunc func);
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_TYPES_CONVERTER_H
