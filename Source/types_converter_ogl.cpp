/*
 *  types_converter_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "prefix_ogl.h"
#include "types_converter_ogl.h"
#include "error.h"

uint TGen::OpenGL::TgenFormatToOpenGL(FormatType format) {
	switch (format) {
		case TGen::TypeFloat:
			return GL_FLOAT;
			
		case TGen::TypeDouble:
			return GL_DOUBLE;
			
		case TGen::TypeInt:
			return GL_INT;
			
		case TGen::TypeShort:
			return GL_SHORT;
			
		case TGen::TypeByte:
			return GL_BYTE;
			
		case TGen::TypeUnsignedInt:
			return GL_UNSIGNED_INT;
			
		case TGen::TypeUnsignedShort:
			return GL_UNSIGNED_SHORT;
			
		case TGen::TypeUnsignedByte:
			return GL_UNSIGNED_BYTE;
			
		case TGen::TypeNone:
			return GL_NONE;		
	}
	
	throw TGen::NotImplemented("TgenFormatToOpenGL", "format not supported");
}

uint TGen::OpenGL::TgenPrimitiveToOpenGL(TGen::PrimitiveType primitive) {
	switch (primitive) {
		case TGen::PrimitiveQuadStrip:
			return GL_QUAD_STRIP;
			
		case TGen::PrimitiveQuads:
			return GL_QUADS;
			
		case TGen::PrimitiveTriangleStrip:
			return GL_TRIANGLE_STRIP;
			
		case TGen::PrimitiveTriangleFan:
			return GL_TRIANGLE_FAN;
			
		case TGen::PrimitiveTriangles:
			return GL_TRIANGLES;
			
		case TGen::PrimitivePoints:
			return GL_POINTS;
			
		case TGen::PrimitiveLineStrip:
			return GL_LINE_STRIP;
			
		case TGen::PrimitiveLineLoop:
			return GL_LINE_LOOP;
			
		case TGen::PrimitiveLines:
			return GL_LINES;
			
		case TGen::PrimitivePolygon:
			return GL_POLYGON;
	}
	
	throw TGen::NotImplemented("TgenPrimitiveToOpenGL", "primitive not supported");	
}

uint TGen::OpenGL::TgenImageFormatToOpenGL(TGen::ImageFormat format) {
	switch (format) {
		case TGen::RGB:
			return GL_RGB;
			
		case TGen::RGB32F:
			return GL_RGB32F_ARB;
			
		case TGen::RGBA:
			return GL_RGBA;
			
		case TGen::RGBA32F:
			return GL_RGBA32F_ARB;
			
		case TGen::BGR:
			return GL_BGR;
			
		case TGen::BGRA:
			return GL_BGRA;
			
		case TGen::LUMINANCE:
			return GL_LUMINANCE;
		
		case TGen::LUMINANCE_ALPHA:
			return GL_LUMINANCE_ALPHA;
	}
	
	throw TGen::NotImplemented("TgenImageFormatToOpenGL", "format not supported");		
}
