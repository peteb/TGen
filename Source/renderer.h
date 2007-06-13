/*
 *  renderer.h
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_H
#define _TGEN_RENDERER_H

#include "color.h"
#include "types.h"
#include "rectangle.h"
#include "matrix4x4.h"

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
		UsageDynamic = 1,
		UsageStatic,
	};
	
	enum PrimitiveType {
		PrimitiveQuads = 1,
		PrimitiveTriangles,
		PrimitiveLines,
	};
	
	class VertexBuffer;
	class VertexStructure;
	
	class Renderer {
	protected:
		Renderer();
		
	public:
		virtual ~Renderer();
		
		virtual void setClearColor(const Color & color);
		virtual TGen::Color getClearColor() const;
		virtual void setViewport(const Rectangle & viewport);
		virtual TGen::Rectangle getViewport() const;
		virtual void setTransform(TransformMode mode, const Matrix4x4 & transformation);
		virtual TGen::Matrix4x4 getTransform(TransformMode mode) const;
		
		virtual void setVertexBuffer(VertexBuffer * buffer) abstract;
		virtual void setColor(const Color & color) abstract;
		
		virtual void Clear(ushort buffers) abstract;
		
		virtual void DrawPrimitive(PrimitiveType type, uint startVertex, uint vertexCount) abstract;
		
		virtual VertexBuffer * CreateVertexBuffer(const VertexStructure & vertstruct, uint size, ushort usage) abstract;
		
	private:
		TGen::Rectangle viewport;
		TGen::Color clearColor;
		TGen::Matrix4x4 projectionMatrix, worldMatrix, textureMatrix;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_H
