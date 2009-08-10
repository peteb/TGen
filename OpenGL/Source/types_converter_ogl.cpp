/*
 *  types_converter_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "prefix_ogl.h"
#include "binder_ogl.h"
#include "types_converter_ogl.h"
#include "error.h"

using TGen::uint;

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
			
		case TGen::DEPTH16:
			return GL_DEPTH_COMPONENT16;
			
		case TGen::DEPTH24:
			return GL_DEPTH_COMPONENT24;
			
		case TGen::DEPTH32:
			return GL_DEPTH_COMPONENT32;
			
		case TGen::DEPTH:
			return GL_DEPTH_COMPONENT;
	}
	
	throw TGen::NotImplemented("TgenImageFormatToOpenGL", "format not supported");		
}

uint TGen::OpenGL::TgenBlendFuncToOpenGL(TGen::BlendFunc blend) {
	switch (blend) {
		case TGen::BlendZero:
			return GL_ZERO;
			
		case TGen::BlendOne:
			return GL_ONE;
			
		case TGen::BlendDestColor:
			return GL_DST_COLOR;
		
		case TGen::BlendOneMinusDestColor:
			return GL_ONE_MINUS_DST_COLOR;
		
		case TGen::BlendSourceAlpha:
			return GL_SRC_ALPHA;
			
		case TGen::BlendOneMinusSourceAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;
		
		case TGen::BlendDestAlpha:
			return GL_DST_ALPHA;
			
		case TGen::BlendOneMinusDestAlpha:
			return GL_ONE_MINUS_DST_ALPHA;
			
		case TGen::BlendSourceColor:
			return GL_SRC_COLOR;
			
		case TGen::BlendOneMinusSourceColor:
			return GL_ONE_MINUS_SRC_COLOR;
	}
	
	throw TGen::NotImplemented("TgenBlendFuncToOpenGL", "func not supported");		
}

uint TGen::OpenGL::TgenTextureWrapToOpenGL(TGen::TextureWrap wrap) {
	switch (wrap) {
		case TGen::TextureWrapClamp:
			return GL_CLAMP;
			
		case TGen::TextureWrapClampToEdge:
			return GL_CLAMP_TO_EDGE;
		
		case TGen::TextureWrapClampToBorder:
			return GL_CLAMP_TO_BORDER;
			
		case TGen::TextureWrapRepeat:
			return GL_REPEAT;
	}
	
	throw TGen::NotImplemented("TgenTextureWrapToOpenGL", "wrap mode not supported");		
}

uint TGen::OpenGL::TgenTextureFilterToOpenGL(TGen::TextureFilter filter) {
	switch (filter) {
		case TGen::TextureFilterNone:
			return GL_NONE;

		case TGen::TextureFilterNearest:
			return GL_NEAREST;
		
		case TGen::TextureFilterLinear:
			return GL_LINEAR;
		
		case TGen::TextureFilterNearestMipmapNearest:	// I don't like this format at all.
			return GL_NEAREST_MIPMAP_NEAREST;
			
		case TGen::TextureFilterLinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
			
		case TGen::TextureFilterNearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;

		case TGen::TextureFilterLinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;			
	}	
	
	return GL_LINEAR;
	//throw TGen::NotImplemented("TgenTextureFilterToOpenGL", "filter not supported");		
}

uint TGen::OpenGL::TgenTextureCoordGenToOpenGL(TGen::TextureCoordGen tcgen) {
	switch (tcgen) {
		case TGen::TextureCoordGenBase:
			return GL_NONE;
			
		case TGen::TextureCoordGenObjectLinear:
			return GL_OBJECT_LINEAR;
			
		case TGen::TextureCoordGenEyeLinear:
			return GL_EYE_LINEAR;
			
		case TGen::TextureCoordGenSphereMap:
			return GL_SPHERE_MAP;
	}
	
	throw TGen::NotImplemented("TgenTextureCoordGenToOpenGL", "gen not supported");			
}

uint TGen::OpenGL::TgenCompareFuncToOpenGL(TGen::CompareFunc func) {
	switch (func) {
		case TGen::CompareNever:
			return GL_NEVER;
			
		case TGen::CompareLess:
			return GL_LESS;
			
		case TGen::CompareEqual:
			return GL_EQUAL;
			
		case TGen::CompareLessOrEqual:
			return GL_LEQUAL;
			
		case TGen::CompareGreater:
			return GL_GREATER;
			
		case TGen::CompareNotEqual:
			return GL_NOTEQUAL;
			
		case TGen::CompareGreaterOrEqual:
			return GL_GEQUAL;
			
		case TGen::CompareAlways:
			return GL_ALWAYS;		
	}
	
	throw TGen::NotImplemented("TgenCompareFuncToOpenGL", "func not supported");			
}

uint TGen::OpenGL::TgenTransformModeToOpenGL(TGen::TransformMode mode) {
	switch (mode) {
		case TGen::TransformWorldView:
			return GL_MODELVIEW;
			
		case TGen::TransformProjection:
			return GL_PROJECTION;
			
		case TGen::TransformTexture:
			return GL_TEXTURE;
			
		case TGen::TransformColor:
			return GL_COLOR;
	}	

	throw TGen::NotImplemented("TgenTransformModeToOpenGL", "transform mode invalid");
}

