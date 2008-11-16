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
#include "prefix_ogl.h"
#include "rendercontext.h"
#include "shader_ogl.h"
#include "shaderprogram_ogl.h"
#include <map>

namespace TGen {
	class VertexStructure;
	class VertexBuffer;
	class IndexBuffer;
	class Texture;
	class FrameBuffer;
	
	class RenderContext;
	
	namespace OpenGL {
		class VertexStructure;
		
		class Renderer : public TGen::Renderer {
		public:
			Renderer();
			~Renderer();
			
			void setClearColor(const TGen::Color & color);
			void setClearDepth(scalar depth);
			void setViewport(const TGen::Rectangle & viewport);
			void setFaceWinding(TGen::FaceWinding winding);
			void setDepthFunc(TGen::CompareFunc compare);
			void setBlendFunc(TGen::BlendFunc srcFunc, TGen::BlendFunc dstFunc);
			void setDepthWrite(bool write);
			void setColorWrite(bool write);
			
			#ifdef _GFX_KEEP_DEPRECATED
			void setVertexBuffer(TGen::VertexBuffer * buffer, TGen::VertexStructure * override = NULL);
			void setIndexBuffer(TGen::IndexBuffer * buffer);
			#endif
			
			
			void setVertexBuffer(TGen::VertexData * buffer, TGen::VertexStructure * override = NULL);
			void setIndexBuffer(TGen::VertexData * buffer);

			void setTexture(int unit, TGen::Texture * texture);
			void setRenderTarget(TGen::FrameBuffer * buffer);
			void setShaderProgram(TGen::ShaderProgram * program);
			TGen::ShaderProgram * getShaderProgram();
			void setLight(int num, const TGen::Light & light);
			void setAmbientLight(const TGen::Color & ambient);
			void setLightMaterial(const TGen::LightMaterial & material);
			
			void setClipPlane(int id, const TGen::Plane3 & plane);
			void disableClipPlane(int id);
			
			void setRenderContext(const TGen::RenderContext & context, TGen::Texture ** textureTypes);
			void setColor(const TGen::Color & color);
			void setTransform(TransformMode mode, const Matrix4x4 & transformation);
			void setTextureTransform(int unit, const Matrix4x4 & transform);
			TGen::Matrix4x4 getTransform(TransformMode mode) const;
			void multiplyTransform(TransformMode mode, const Matrix4x4 & transform);
			
			void clearBuffers(ushort buffers);
			
			void drawPrimitive(TGen::PrimitiveType type, uint startVertex, uint vertexCount);
			void drawIndexedPrimitive(TGen::PrimitiveType type, uint startIndex, uint indexCount);
			
			TGen::VertexData * createVertexData(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
			void removeVertexData(TGen::VertexData * data);
			
			#ifdef _GFX_KEEP_DEPRECATED
			TGen::VertexBuffer * createVertexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
			TGen::IndexBuffer * createIndexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
			#endif
			
			TGen::Texture * createTexture(const TGen::Rectangle & size, TGen::ImageFormat components, TGen::FormatType componentFormat, uint flags);
			TGen::Texture * createTexture(const TGen::Image & image, TGen::ImageFormat components, uint flags);
			TGen::FrameBuffer * createFrameBuffer();
			TGen::Shader * createVertexShader(const char * code);
			TGen::Shader * createFragmentShader(const char * code);
			TGen::Shader * createGeometryShader(const char * code);
			TGen::OpenGL::ShaderProgram * createShaderProgram();
			TGen::OpenGL::ShaderProgram * createShaderProgram(const char * code);
			
			bool throwErrors();
			
			static bool isExtensionAvailable(const std::string & extension);
			TGen::FrameBuffer * getBoundFramebuffer();
			
		private:
			void parseExtensions();
			void readCaps();
			void checkCompatibility();

			TGen::Texture * createTexture(const void * data, const TGen::Rectangle & size, TGen::ImageFormat format, TGen::ImageFormat components, TGen::FormatType componentFormat, uint flags);
			TGen::Texture * create2DTexture(const void * data, const TGen::Rectangle & size, GLenum internalFormat, GLenum format, GLenum dataType, uint flags);
			TGen::Texture * createRectTexture(const void * data, const TGen::Rectangle & size,  GLenum internalFormat, GLenum inFormat, GLenum dataType, uint flags);
			GLenum getInternalFormat(TGen::ImageFormat components, uint flags);
			
			TGen::OpenGL::Shader * createShader(const char * code, int type);

			void setTextureCoordGen(TGen::TextureCoordGen genU, TGen::TextureCoordGen genV);
			
			
			void applyVertexStructure(const TGen::VertexStructure & vertstruct);
			
		private:			
			typedef std::map<std::string, bool> ExtensionMap;
			static ExtensionMap extensionsAvailable;
			
			bool colorFromVertex, hasColorArray;
			uint indexBufferFormat, indexBufferFormatSize;
			TGen::ShaderProgram * lastShader;
			
			#ifdef _GFX_KEEP_DEPRECATED
			TGen::VertexBuffer * lastVb;
			TGen::IndexBuffer * lastIb;
			#endif
			
			GLuint lastFBO;
			GLuint lastVb1, lastIb1;
			uint vbReadOffset, ibReadOffset;
			TGen::VertexData * lastVb2, * lastIb2;
			TGen::FrameBuffer * lastBoundFBO;
			
			// information to reduce state switches
			GLenum activeTextureUnit;
			GLenum * textureUnitTargets;
			TGen::Texture ** textureUnitTextures;

			// vertex structure:
			bool hasCoordElements, hasNormalElements, hasColorElements, hasEdgeElements;
			bool hasTexCoordUnitElements[8];

			// render context
			TGen::RenderContext lastContext;
			TGen::TextureCoordGen textureCoordGenU, textureCoordGenV;
			
			TGen::FaceWinding currentWinding;
		};
		
	} // !OpenGL	
} // !TGen

#endif // !_TGEN_OPENGL_RENDERER_H
