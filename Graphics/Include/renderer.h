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
#include "vertexsource.h"
#include "renderstatistics.h"
#include <vector>

//#define _GFX_KEEP_DEPRECATED

namespace TGen {
	class VertexBuffer;
	class IndexBuffer;
	class VertexStructure;
	class Texture;
	class FrameBuffer;
	class Shader;
	class ShaderProgram;
	class RenderContext;
	class Plane3;
	class Light;
	class LightMaterial;
	
	class Renderer : public TGen::VertexDataSource {
	protected:
		Renderer();
		
	public:
		virtual ~Renderer();
		
		TGen::RendererCaps const & getCaps() const;
		TGen::RenderStatistics & getStatistics();
		
		virtual TGen::Color getClearColor() const;
		virtual TGen::Rectangle getViewport() const;
		virtual TGen::Matrix4x4 getTransform(TransformMode mode) const;

		virtual void setClearColor(const Color & color);
		virtual void setClearDepth(scalar depth) abstract;
		virtual void setViewport(const Rectangle & viewport);
		virtual void setTransform(TransformMode mode, const Matrix4x4 & transformation);
		virtual void setTextureTransform(int unit, const Matrix4x4 & transform) abstract;
		virtual void multiplyTransform(TransformMode mode, const Matrix4x4 & transform);
		
		virtual void setDepthFunc(TGen::CompareFunc compare) abstract;
		virtual void setBlendFunc(TGen::BlendFunc srcFunc, TGen::BlendFunc dstFunc) abstract;
		virtual void setDepthWrite(bool write) abstract;
		virtual void setColorWrite(bool write) abstract;
		
		#ifdef _GFX_KEEP_DEPRECATED
		virtual void setVertexBuffer(VertexBuffer * buffer, VertexStructure * override = NULL) abstract;
		virtual void setIndexBuffer(IndexBuffer * buffer) abstract;
		#endif
		
		virtual void setVertexBuffer(VertexData * buffer, VertexStructure * override = NULL) abstract;
		virtual void setIndexBuffer(VertexData * buffer) abstract;
		
		virtual void setTexture(int unit, Texture * texture) abstract;
		virtual void setRenderTarget(FrameBuffer * buffer) abstract;
		virtual void setShaderProgram(ShaderProgram * program) abstract;
		virtual ShaderProgram * getShaderProgram() abstract;
		
		virtual void disableClipPlane(int id) abstract;
		virtual void setClipPlane(int id, const TGen::Plane3 & plane) abstract;
		virtual void setColor(const Color & color) abstract;
		virtual void setRenderContext(const RenderContext & context, TGen::Texture ** textureTypes) abstract;
		virtual void setFaceWinding(TGen::FaceWinding winding) abstract;

		virtual void setLight(int num, const TGen::Light & light) abstract;
		virtual void setAmbientLight(const TGen::Color & ambient) abstract;
		virtual void setLightMaterial(const TGen::LightMaterial & material) abstract;
		
		virtual void clearBuffers(ushort buffers) abstract;
		
		virtual void drawPrimitive(TGen::PrimitiveType type, uint startVertex, uint vertexCount) abstract;
		virtual void drawIndexedPrimitive(TGen::PrimitiveType type, uint startIndex, uint indexCount) abstract;

		#ifdef _GFX_KEEP_DEPRECATED
		virtual VertexBuffer * createVertexBuffer(const VertexStructure & vertstruct, uint size, ushort usage) abstract;
		virtual IndexBuffer * createIndexBuffer(const VertexStructure & vertstruct, uint size, ushort usage) abstract;
		#endif
		
		
		virtual VertexData * createVertexData(const VertexStructure & vertstruct, uint size, ushort usage) abstract;
		virtual Texture * createTexture(const TGen::Rectangle & size, TGen::ImageFormat components, TGen::FormatType componentFormat, uint flags = 0) abstract;
		virtual Texture * createTexture(const TGen::Image & image, TGen::ImageFormat components, uint flags = 0) abstract;
		virtual FrameBuffer * createFrameBuffer() abstract;
		virtual Shader * createVertexShader(const char * code) abstract;
		virtual Shader * createFragmentShader(const char * code) abstract;
		virtual Shader * createGeometryShader(const char * code) abstract;
		virtual ShaderProgram * createShaderProgram() abstract;
		virtual ShaderProgram * createShaderProgram(const char * code) abstract;
		
		virtual bool throwErrors() abstract;
		
	protected:
		TGen::RenderStatistics stats;
		TGen::RendererCaps caps;
		TGen::Rectangle viewport;
		TGen::Color clearColor;
		TGen::Matrix4x4 projectionMatrix, worldMatrix;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_H
