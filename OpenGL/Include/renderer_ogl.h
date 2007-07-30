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
#include <map>

#ifndef GL_RGBA32F_ARB
	#define GL_RGBA32F_ARB                      0x8814
	#define GL_RGB32F_ARB                       0x8815
#endif

#ifndef GL_GEOMETRY_SHADER_EXT
	#define GL_GEOMETRY_SHADER_EXT              0x8DD9
#endif

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
	#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT                   0x83F0
	#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT                  0x83F1
	#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT                  0x83F2
	#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT                  0x83F3
#endif
#define GL_TEXTURE_IMAGE_SIZE_ARB 0x86A0

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
			void setViewport(const TGen::Rectangle & viewport);
			void setVertexBuffer(TGen::VertexBuffer * buffer);
			void setIndexBuffer(TGen::IndexBuffer * buffer);
			void setTexture(int unit, TGen::Texture * texture);
			void setRenderTarget(TGen::FrameBuffer * buffer);
			void setShaderProgram(TGen::ShaderProgram * program);
			
			void setRenderContext(const TGen::RenderContext & context, TGen::Texture ** textureTypes);
			void setColor(const TGen::Color & color);
			void setTransform(TransformMode mode, const Matrix4x4 & transformation);
			TGen::Matrix4x4 getTransform(TransformMode mode) const;
			
			void Clear(ushort buffers);
			
			void DrawPrimitive(TGen::PrimitiveType type, uint startVertex, uint vertexCount);
			void DrawIndexedPrimitive(TGen::PrimitiveType type, uint startIndex, uint indexCount);
			
			TGen::VertexBuffer * CreateVertexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
			TGen::IndexBuffer * CreateIndexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
			TGen::Texture * CreateTexture(const TGen::Rectangle & size, TGen::ImageFormat components, TGen::FormatType componentFormat, uint flags);
			TGen::Texture * CreateTexture(const TGen::Image & image, TGen::ImageFormat components, uint flags);
			TGen::FrameBuffer * CreateFrameBuffer();
			TGen::Shader * CreateVertexShader(const char * code);
			TGen::Shader * CreateFragmentShader(const char * code);
			TGen::Shader * CreateGeometryShader(const char * code);
			TGen::ShaderProgram * CreateShaderProgram();
			TGen::ShaderProgram * CreateShaderProgram(const char * code);
			
			static bool isExtensionAvailable(const std::string & extension);
			
		private:
			void ParseExtensions();
			void ReadCaps();
			void CheckCompatibility();
			
			TGen::Texture * CreateTexture(const void * data, const TGen::Rectangle & size, TGen::ImageFormat format, TGen::ImageFormat components, TGen::FormatType componentFormat, uint flags);
			TGen::Shader * CreateShader(const char * code, int type);

			void setTextureCoordGen(TGen::TextureCoordGen genU, TGen::TextureCoordGen genV);
			void setDepthFunc(TGen::CompareFunc compare);
			
			void ApplyVertexStructure(const TGen::VertexStructure & vertstruct);
			
		private:			
			typedef std::map<std::string, bool> ExtensionMap;
			static ExtensionMap extensionsAvailable;
			
			bool colorFromVertex;
			uint indexBufferFormat;
			TGen::VertexBuffer * lastVb;
			TGen::IndexBuffer * lastIb;
		};
		
	} // !OpenGL	
} // !TGen

#endif // !_TGEN_OPENGL_RENDERER_H
