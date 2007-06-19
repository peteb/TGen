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
#include "renderer_types.h"
#include "renderer_caps.h"

namespace TGen {
	class VertexBuffer;
	class IndexBuffer;
	class VertexStructure;
	
	class Renderer {
	protected:
		Renderer();
		
	public:
		virtual ~Renderer();
		
		TGen::RendererCaps const & getCaps() const;
		
		virtual void setClearColor(const Color & color);
		virtual TGen::Color getClearColor() const;
		virtual void setViewport(const Rectangle & viewport);
		virtual TGen::Rectangle getViewport() const;
		virtual void setTransform(TransformMode mode, const Matrix4x4 & transformation);
		virtual TGen::Matrix4x4 getTransform(TransformMode mode) const;
		
		virtual void setVertexBuffer(VertexBuffer * buffer) abstract;
		virtual void setIndexBuffer(IndexBuffer * buffer) abstract;
		virtual void setColor(const Color & color) abstract;
		
		virtual void Clear(ushort buffers) abstract;
		
		virtual void DrawPrimitive(PrimitiveType type, uint startVertex, uint vertexCount) abstract;
		virtual void DrawIndexedPrimitive(TGen::PrimitiveType type, uint startIndex, uint indexCount) abstract;
		
		virtual VertexBuffer * CreateVertexBuffer(const VertexStructure & vertstruct, uint size, ushort usage) abstract;
		virtual IndexBuffer * CreateIndexBuffer(const VertexStructure & vertstruct, uint size, ushort usage) abstract;
		
	protected:
		TGen::RendererCaps caps;
		TGen::Rectangle viewport;
		TGen::Color clearColor;
		TGen::Matrix4x4 projectionMatrix, worldMatrix, textureMatrix;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_H
