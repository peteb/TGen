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
	enum TextureCreationFlags {
		TextureCompressed =				0x0001,		// first four bits are compression. 0xF
		TextureS3TCDXT1Compressed =		0x0003,
		TextureS3TCDXT3Compressed =		0x0005,
		TextureS3TCDXT5Compressed =		0x0007,
	};
	
	class VertexBuffer;
	class IndexBuffer;
	class VertexStructure;
	class Texture;
	class FrameBuffer;
	class Shader;
	class ShaderProgram;
	class RenderContext;
	
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
		virtual void setTexture(int unit, Texture * texture) abstract;
		virtual void setRenderTarget(FrameBuffer * buffer) abstract;
		virtual void setShaderProgram(ShaderProgram * program) abstract;
		
		virtual void setColor(const Color & color) abstract;
		virtual void setRenderContext(const RenderContext & context) abstract;
		
		virtual void Clear(ushort buffers) abstract;
		
		virtual void DrawPrimitive(PrimitiveType type, uint startVertex, uint vertexCount) abstract;
		virtual void DrawIndexedPrimitive(TGen::PrimitiveType type, uint startIndex, uint indexCount) abstract;
		
		virtual VertexBuffer * CreateVertexBuffer(const VertexStructure & vertstruct, uint size, ushort usage) abstract;
		virtual IndexBuffer * CreateIndexBuffer(const VertexStructure & vertstruct, uint size, ushort usage) abstract;
		virtual Texture * CreateTexture(const TGen::Rectangle & size, TGen::ImageFormat components, TGen::FormatType componentFormat, uint flags = 0) abstract;
		virtual Texture * CreateTexture(const TGen::Image & image, TGen::ImageFormat components, uint flags = 0) abstract;
		virtual FrameBuffer * CreateFrameBuffer() abstract;
		virtual Shader * CreateVertexShader(const char * code) abstract;
		virtual Shader * CreateFragmentShader(const char * code) abstract;
		virtual Shader * CreateGeometryShader(const char * code) abstract;
		virtual ShaderProgram * CreateShaderProgram() abstract;
		virtual ShaderProgram * CreateShaderProgram(char * code) abstract;
		
	protected:
		TGen::RendererCaps caps;
		TGen::Rectangle viewport;
		TGen::Color clearColor;
		TGen::Matrix4x4 projectionMatrix, worldMatrix, textureMatrix;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_H
