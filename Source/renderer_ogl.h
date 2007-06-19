/*
 *  renderer_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_RENDERER_H
#define _TGEN_OPENGL_RENDERER_H

#include "renderer.h"

namespace TGen {
	class VertexStructure;
	class VertexBuffer;
	class IndexBuffer;
	
	namespace OpenGL {
		class VertexStructure;
		
		class Renderer : public TGen::Renderer {
		public:
			Renderer();
			~Renderer();
			
			void setClearColor(const TGen::Color & color);
			void setViewport(const TGen::Rectangle & viewport);
			void setVertexBuffer(TGen::VertexBuffer * buffer);
			void setIndexBuffer(TGen::IndexBuffer * buffer);
			void setColor(const TGen::Color & color);
			void setTransform(TransformMode mode, const Matrix4x4 & transformation);
			TGen::Matrix4x4 getTransform(TransformMode mode) const;
			
			void Clear(ushort buffers);
			
			void DrawPrimitive(TGen::PrimitiveType type, uint startVertex, uint vertexCount);
			void DrawIndexedPrimitive(TGen::PrimitiveType type, uint startIndex, uint indexCount);
			
			TGen::VertexBuffer * CreateVertexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
			TGen::IndexBuffer * CreateIndexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
			
		private:
			void ApplyVertexStructure(const TGen::VertexStructure & vertstruct);
			
			
			uint indexBufferFormat;
		};
		
	} // !OpenGL	
} // !TGen

#endif // !_TGEN_OPENGL_RENDERER_H
