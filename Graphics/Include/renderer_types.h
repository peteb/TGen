/*
 *  renderer_types.h
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_TYPES_H
#define _TGEN_RENDERER_TYPES_H

namespace TGen {
	enum TextureCreationFlags {
		TextureCompressed =				0x0001,		// first four bits are compression. 0xF
		TextureS3TCDXT1Compressed =		0x0003,
		TextureS3TCDXT3Compressed =		0x0005,
		TextureS3TCDXT5Compressed =		0x0007,
		TextureNoMipmaps =				0x0010,
	};
		
	enum BufferType {
		ColorBuffer		= 0x0001,
		DepthBuffer		= 0x0002,
		StencilBuffer	= 0x0004,
	};
	
	enum TransformMode {
		TransformProjection,
		TransformWorldView,
		TransformTexture,
		TransformColor,
	};
	
	enum Usage {
		UsageStream		= 0x0001,
		UsageDynamic	= 0x0002,
		UsageStatic		= 0x0004,
		UsageRead		= 0x0010,
		UsageCopy		= 0x0020,
		UsageDraw		= 0x0040,
	};
	
	enum PrimitiveType {
		PrimitiveQuadStrip = 1,
		PrimitiveQuads,
		PrimitiveTriangleStrip,
		PrimitiveTriangleFan,
		PrimitiveTriangles,		
		PrimitivePoints,
		PrimitiveLineStrip,
		PrimitiveLineLoop,
		PrimitiveLines,
		PrimitivePolygon,
		
	};

	enum TextureWrap {
		TextureWrapClamp = 1,
		TextureWrapClampToEdge,
		TextureWrapClampToBorder,
		TextureWrapRepeat,
	};
	
	enum TextureFilter {
		TextureFilterNone = 0,
		TextureFilterNearest = 1,
		TextureFilterLinear,
		TextureFilterNearestMipmapNearest,
		TextureFilterLinearMipmapNearest,
		TextureFilterNearestMipmapLinear,
		TextureFilterLinearMipmapLinear,
	};
	
	enum TextureCoordGen {
		TextureCoordGenBase = 0,
		TextureCoordGenObjectLinear,
		TextureCoordGenEyeLinear,
		TextureCoordGenSphereMap,
	};
	
	enum CompareFunc {
		CompareNever = 1,
		CompareLess,
		CompareEqual,
		CompareLessOrEqual,
		CompareGreater,
		CompareNotEqual,
		CompareGreaterOrEqual,
		CompareAlways,
	};
	
	enum PolygonFaceMode {
		PolygonFaceFill = 1,
		PolygonFaceLines,
		PolygonFacePoints,
		PolygonFaceCull,		
	};
	
	enum BlendFunc {
		BlendZero = 1,
		BlendOne,
		BlendDestColor,
		BlendOneMinusDestColor,
		BlendSourceAlpha,
		BlendOneMinusSourceAlpha,
		BlendDestAlpha,
		BlendOneMinusDestAlpha,
		BlendSourceColor,
		BlendOneMinusSourceColor,		
	};
	
	enum LockFlags {
		LockWrite		= 0x0001,
		LockRead		= 0x0002,
		LockReadWrite	= 0x0003,
		LockDiscard		= 0x0010,
	};
	
	
} // !TGen

#endif // !_TGEN_RENDERER_TYPES_H
