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

	enum Buffers {
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
		UsageStream = 1,
		UsageDynamic,
		UsageStatic,
		UsageRead,
		UsageCopy,
		UsageDraw,
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
	
	enum LockFlags {
		LockWrite		= 0x0001,
		LockRead		= 0x0002,
		LockReadWrite	= 0x0003,
		LockDiscard		= 0x0010,
	};
	
	
} // !TGen

#endif // !_TGEN_RENDERER_TYPES_H
