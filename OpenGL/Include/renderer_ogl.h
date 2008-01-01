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

#define _OGL_DEBUG

#ifdef _OGL_DEBUG
	#define STAT_ADD(x) stats.addStat(x)
	#define STAT_ADD_SHADER(x) stats.addShader(x)
	#define STAT_ADD_TEXTURE(x, y) stats.addTexture(x, y)
#else
	#define STAT_ADD(x)
	#define STAT_ADD_SHADER(x)
	#define STAT_ADD_TEXTURE(x, y)
#endif

#include "renderer.h"
#include "prefix_ogl.h"
#include "rendercontext.h"
#include <map>

namespace TGen {
	class VertexStructure;
	class VertexBuffer;
	class IndexBuffer;
	class Texture;
	class FrameBuffer;
	class Shader;
	class ShaderProgram;
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
			void setVertexBuffer(TGen::VertexBuffer * buffer, TGen::VertexStructure * override = NULL);
			void setIndexBuffer(TGen::IndexBuffer * buffer);
			void setVertexBuffer(TGen::VertexData * buffer, TGen::VertexStructure * override = NULL);
			void setIndexBuffer(TGen::VertexData * buffer);

			void setTexture(int unit, TGen::Texture * texture);
			void setRenderTarget(TGen::FrameBuffer * buffer);
			void setShaderProgram(TGen::ShaderProgram * program);
			void setLight(int num, const TGen::Light & light);
			void setAmbientLight(const TGen::Color & ambient);
			
			void setClipPlane(int id, const TGen::Plane3 & plane);
			void disableClipPlane(int id);
			
			void setRenderContext(const TGen::RenderContext & context, TGen::Texture ** textureTypes);
			void setColor(const TGen::Color & color);
			void setTransform(TransformMode mode, const Matrix4x4 & transformation);
			TGen::Matrix4x4 getTransform(TransformMode mode) const;
			void multiplyTransform(TransformMode mode, const Matrix4x4 & transform);
			
			void clearBuffers(ushort buffers);
			
			void drawPrimitive(TGen::PrimitiveType type, uint startVertex, uint vertexCount);
			void drawIndexedPrimitive(TGen::PrimitiveType type, uint startIndex, uint indexCount);
			
			TGen::VertexData * createVertexData(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
			void removeVertexData(TGen::VertexData * data);
			TGen::VertexBuffer * createVertexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
			TGen::IndexBuffer * createIndexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
			TGen::Texture * createTexture(const TGen::Rectangle & size, TGen::ImageFormat components, TGen::FormatType componentFormat, uint flags);
			TGen::Texture * createTexture(const TGen::Image & image, TGen::ImageFormat components, uint flags);
			TGen::FrameBuffer * createFrameBuffer();
			TGen::Shader * createVertexShader(const char * code);
			TGen::Shader * createFragmentShader(const char * code);
			TGen::Shader * createGeometryShader(const char * code);
			TGen::ShaderProgram * createShaderProgram();
			TGen::ShaderProgram * createShaderProgram(const char * code);
			
			bool throwErrors();
			
			static bool isExtensionAvailable(const std::string & extension);
			
		private:
			void parseExtensions();
			void readCaps();
			void checkCompatibility();

			TGen::Texture * createTexture(const void * data, const TGen::Rectangle & size, TGen::ImageFormat format, TGen::ImageFormat components, TGen::FormatType componentFormat, uint flags);
			TGen::Texture * create2DTexture(const void * data, const TGen::Rectangle & size, GLenum internalFormat, GLenum format, GLenum dataType, uint flags);
			TGen::Texture * createRectTexture(const void * data, const TGen::Rectangle & size,  GLenum internalFormat, GLenum inFormat, GLenum dataType, uint flags);
			GLenum getInternalFormat(TGen::ImageFormat components, uint flags);
			
			TGen::Shader * createShader(const char * code, int type);

			void setTextureCoordGen(TGen::TextureCoordGen genU, TGen::TextureCoordGen genV);
			void setDepthFunc(TGen::CompareFunc compare);
			
			void applyVertexStructure(const TGen::VertexStructure & vertstruct);
			
		private:			
			typedef std::map<std::string, bool> ExtensionMap;
			static ExtensionMap extensionsAvailable;
			
			bool colorFromVertex, hasColorArray;
			uint indexBufferFormat, indexBufferFormatSize;
			TGen::ShaderProgram * lastShader;
			TGen::VertexBuffer * lastVb;
			TGen::IndexBuffer * lastIb;
			GLuint lastVb1, lastIb1;
			uint vbReadOffset, ibReadOffset;
			TGen::VertexData * lastVb2, * lastIb2;
			
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
			
		};
		
	} // !OpenGL	
} // !TGen

#endif // !_TGEN_OPENGL_RENDERER_H
