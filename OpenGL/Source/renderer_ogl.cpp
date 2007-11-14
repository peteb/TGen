/*
 *  renderer_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "prefix_ogl.h"
#include "binder_ogl.h"
#include "renderer_ogl.h"
#include "vertexstructure.h"
#include "vertexbuffer_ogl.h"
#include "indexbuffer_ogl.h"
#include "texture_ogl.h"
#include "error.h"
#include "types_converter_ogl.h"
#include "framebuffer_ogl.h"
#include "shader_ogl.h"
#include "shaderprogram_ogl.h"
#include <tgen_graphics.h>
#include <tgen_core.h>
#include <iostream>

TGen::OpenGL::Renderer::ExtensionMap TGen::OpenGL::Renderer::extensionsAvailable;

TGen::OpenGL::Renderer::Renderer()
	: colorFromVertex(true)
	, hasColorArray(false)
	, lastVb(NULL)
	, lastIb(NULL)
{
	TGen::OpenGL::BindFunctions();	// might be needed if we're running on a sucky platform!
	
	parseExtensions();
	readCaps();
	checkCompatibility();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

TGen::OpenGL::Renderer::~Renderer() {}

void TGen::OpenGL::Renderer::readCaps() {
	GLint viewportDims[2];
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, reinterpret_cast<GLint *>(&caps.maxTextureUnits));
	glGetIntegerv(GL_MAX_LIGHTS, reinterpret_cast<GLint *>(&caps.maxActiveLights));
	glGetIntegerv(GL_MAX_CLIP_PLANES, reinterpret_cast<GLint *>(&caps.maxClipPlanes));
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, reinterpret_cast<GLint *>(&caps.maxIndexBufferIndicies));
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, reinterpret_cast<GLint *>(&caps.maxVertexBufferVertices));
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, reinterpret_cast<GLint *>(&caps.maxTextureSize));
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewportDims);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, reinterpret_cast<GLint *>(&caps.maxFrameBufferColorAttachments));
	
	caps.maxViewportSize = TGen::Rectangle(viewportDims[0], viewportDims[1]);
	
	if (glGetString(GL_SHADING_LANGUAGE_VERSION))
		caps.shadingLanguageVersion = (char *)(glGetString(GL_SHADING_LANGUAGE_VERSION));
	else
		caps.shadingLanguageVersion = "N/A";
	
	caps.driverName = "OpenGL ";
	caps.driverName += (char *)glGetString(GL_VERSION);
	caps.rendererName = (char *)glGetString(GL_RENDERER);
	caps.driverVendor = (char *)glGetString(GL_VENDOR);
	caps.vertexShader = isExtensionAvailable("GL_ARB_vertex_shader");	// GL_ARB_vertex_program?
	caps.fragmentShader = isExtensionAvailable("GL_ARB_fragment_shader");
	caps.geometryShader = isExtensionAvailable("EXT_geometry_shader4");
	caps.framebuffer = isExtensionAvailable("GL_EXT_framebuffer_object");
	caps.multitexturing = isExtensionAvailable("GL_ARB_multitexture");
}

void TGen::OpenGL::Renderer::parseExtensions() {
	std::string extensions = (char *)glGetString(GL_EXTENSIONS);

	for (std::string::size_type pos = 0; pos < extensions.size(); ) {
		std::string::size_type next = extensions.find(" ", pos);
		if (next == std::string::npos)
			next = extensions.size();
		
		std::string extension = extensions.substr(pos, next - pos);
		extensionsAvailable[extension] = true;
		
		pos = next + 1;
	}
}

void TGen::OpenGL::Renderer::checkCompatibility() {
	bool compatible = true;
	
	if (!isExtensionAvailable("GL_ARB_vertex_buffer_object"))
		compatible = false;
	
	if (!compatible)
		throw TGen::RuntimeException("OpenGL::Renderer::CheckCompatibility", "not compatible!");
}

bool TGen::OpenGL::Renderer::isExtensionAvailable(const std::string & extension) {
	ExtensionMap::iterator iter = extensionsAvailable.find(extension);
	if (iter == extensionsAvailable.end())
		return false;
	
	return true;
}

void TGen::OpenGL::Renderer::setClearColor(const TGen::Color & color) {
	TGen::Renderer::setClearColor(color);

	glClearColor(color.r, color.g, color.g, color.a);
}

void TGen::OpenGL::Renderer::setClearDepth(scalar depth) {
	glClearDepth(depth);
}

void TGen::OpenGL::Renderer::setViewport(const TGen::Rectangle & viewport) {
	TGen::Vector2 upperLeft = viewport.getMin();
	
	glViewport(upperLeft.x, upperLeft.y, viewport.width, viewport.height);	// NOTE: ogl origo is lower-left corner
}

void TGen::OpenGL::Renderer::setColor(const TGen::Color & color) {
	glColor4f(color.r, color.g, color.b, color.a);
}

void TGen::OpenGL::Renderer::clearBuffers(ushort buffers) {
	GLbitfield fixed = 0;
	
	if (buffers & TGen::ColorBuffer)
		fixed |= GL_COLOR_BUFFER_BIT;
	if (buffers & TGen::DepthBuffer)
		fixed |= GL_DEPTH_BUFFER_BIT;
	if (buffers & TGen::StencilBuffer)
		fixed |= GL_STENCIL_BUFFER_BIT;
	
	glClear(fixed);
}

void TGen::OpenGL::Renderer::setTransform(TGen::TransformMode mode, const TGen::Matrix4x4 & transformation) {
	glMatrixMode(TgenTransformModeToOpenGL(mode));

	if (mode != TransformTexture)
		TGen::Renderer::setTransform(mode, transformation);
	
	//float elements[16];
	//transformation.get4x4(elements);
	
	glLoadMatrixf((GLfloat *)transformation.elements);	
}

TGen::Matrix4x4 TGen::OpenGL::Renderer::getTransform(TGen::TransformMode mode) const {
	uint fixedTransform = 0;
	
	switch (mode) {
		case TGen::TransformWorldView:
			fixedTransform = GL_MODELVIEW_MATRIX;
			break;
			
		case TGen::TransformProjection:
			fixedTransform = GL_PROJECTION_MATRIX;
			break;
			
		case TGen::TransformTexture:
			fixedTransform = GL_TEXTURE_MATRIX;
			break;
			
		case TGen::TransformColor:
			fixedTransform = GL_COLOR_MATRIX;
			break;
			
		default:
			throw TGen::NotImplemented("OpenGL::Renderer::getTransform", "transform mode invalid");		
	}
	
	TGen::Matrix4x4 ret;
	glGetFloatv(fixedTransform, (GLfloat *)ret.elements);  // TODO: SYNCPOINT!!! cache!
	
	return ret;
}

void TGen::OpenGL::Renderer::multiplyTransform(TGen::TransformMode mode, const TGen::Matrix4x4 & transform) {
	glMatrixMode(TgenTransformModeToOpenGL(mode));
	glMultMatrixf((GLfloat *)transform.elements);		
}

TGen::VertexBuffer * TGen::OpenGL::Renderer::createVertexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage) {
	GLuint newVBO = 0;
	glGenBuffersARB(1, &newVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, newVBO);
	
	DEBUG_PRINT("[opengl]: created vertex buffer " << newVBO);
	
	GLenum fixedUsage = 0;
	
	if (usage & TGen::UsageStream) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_STREAM_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_STREAM_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_STREAM_READ_ARB;
		else
			fixedUsage = GL_STREAM_DRAW_ARB;
	}
	else if (usage & TGen::UsageDynamic) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_DYNAMIC_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_DYNAMIC_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_DYNAMIC_READ_ARB;
		else
			fixedUsage = GL_DYNAMIC_DRAW_ARB;
	}
	else if (usage & TGen::UsageStatic) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_STATIC_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_STATIC_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_STATIC_READ_ARB;
		else
			fixedUsage = GL_STATIC_DRAW_ARB;
	}
	
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, NULL, fixedUsage);

	return new TGen::OpenGL::VertexBuffer(*this, vertstruct, size, fixedUsage, newVBO);
}

TGen::IndexBuffer * TGen::OpenGL::Renderer::createIndexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage) {
	GLuint newVBO = 0;
	glGenBuffersARB(1, &newVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, newVBO);
	
	DEBUG_PRINT("[opengl]: created index buffer " << newVBO);
	
	GLenum fixedUsage = 0;
	
	if (usage & TGen::UsageStream) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_STREAM_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_STREAM_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_STREAM_READ_ARB;
		else
			fixedUsage = GL_STREAM_DRAW_ARB;
	}
	else if (usage & TGen::UsageDynamic) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_DYNAMIC_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_DYNAMIC_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_DYNAMIC_READ_ARB;
		else
			fixedUsage = GL_DYNAMIC_DRAW_ARB;
	}
	else if (usage & TGen::UsageStatic) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_STATIC_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_STATIC_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_STATIC_READ_ARB;
		else
			fixedUsage = GL_STATIC_DRAW_ARB;
	}
	
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, NULL, fixedUsage);
	
	return new TGen::OpenGL::IndexBuffer(*this, vertstruct, size, fixedUsage, newVBO);	
}

TGen::Texture * TGen::OpenGL::Renderer::createTexture(const TGen::Rectangle & size, TGen::ImageFormat components, TGen::FormatType componentFormat, uint flags) {
	return createTexture(NULL, size, components, components, componentFormat, flags);
}

TGen::Texture * TGen::OpenGL::Renderer::createTexture(const TGen::Image & image, TGen::ImageFormat components, uint flags) {
	return createTexture(image.getData(), image.getSize(), image.getFormat(), components, image.getComponentFormat(), flags);
}

TGen::Texture * TGen::OpenGL::Renderer::createTexture(const void * data, const TGen::Rectangle & size, TGen::ImageFormat format, TGen::ImageFormat components, TGen::FormatType componentFormat, uint flags) {
	GLuint newTex = 0;
	
	
	GLenum imgFormat = TGen::OpenGL::TgenImageFormatToOpenGL(format);
	GLenum type = TGen::OpenGL::TgenFormatToOpenGL(componentFormat);
	GLenum internalFormat = 0;
	GLint prevTex = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTex);
	
	glGenTextures(1, &newTex);
	glBindTexture(GL_TEXTURE_2D, newTex);
	
	bool generateMipmaps = !(flags & TGen::TextureNoMipmaps);
	
	if (flags & TGen::TextureCompressed) {
		switch (components) {
			case TGen::RGB:
				if ((flags & 0xF) == TGen::TextureS3TCDXT1Compressed)
					internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
				else
					throw TGen::RuntimeException("OpenGL::Renderer::CreateTexture", "component format is not compatible with this compression");
				break;
				
			case TGen::RGBA:
				if ((flags & 0xF) == TGen::TextureS3TCDXT1Compressed)
					internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				else if ((flags & 0xF) == TGen::TextureS3TCDXT3Compressed)
					internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				else if ((flags & 0xF) == TGen::TextureS3TCDXT5Compressed)
					internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				else
					throw TGen::RuntimeException("OpenGL::Renderer::CreateTexture", "component format is not compatible with this compression");
				
				break;
				
			default:
				throw TGen::RuntimeException("OpenGL::Renderer::CreateTexture", "component format is not compatible with this compression");
		}
	}
	else {
		internalFormat = TGen::OpenGL::TgenImageFormatToOpenGL(components);
	}
	
	if (generateMipmaps) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, size.width, size.height, imgFormat, type, data);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.width, size.height, 0, imgFormat, type, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	}
	
	DEBUG_PRINT("[opengl]: created texture " << newTex);
	glBindTexture(GL_TEXTURE_2D, prevTex);
	
	return new TGen::OpenGL::Texture(*this, newTex, size);
}

void TGen::OpenGL::Renderer::setVertexBuffer(TGen::VertexBuffer * buffer, TGen::VertexStructure * override) {
	if (!buffer) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);		
	}
	else if (buffer != lastVb) {
		TGen::OpenGL::VertexBuffer * vb = static_cast<TGen::OpenGL::VertexBuffer *>(buffer);
		if (!vb) // NOTE: this is never thrown, static_cast!
			throw TGen::RuntimeException("OpenGL::Renderer::setVertexBuffer", "vertex buffer is not a valid OpenGL buffer");
		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vb->vboId);
		
		if (override)
			applyVertexStructure(*override);			
		else
			applyVertexStructure(buffer->getVertexStructure());
		
		lastVb = buffer;
	}
}

void TGen::OpenGL::Renderer::setIndexBuffer(TGen::IndexBuffer * buffer) {	// You HAVE to call these before DrawPrimitive*, no checking is done
	if (!buffer) {
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}
	else if (buffer != lastIb) {
		TGen::OpenGL::IndexBuffer * ib = static_cast<TGen::OpenGL::IndexBuffer *>(buffer);
		if (!ib) // NOTE: this is never thrown, static_cast!
			throw TGen::RuntimeException("OpenGL::Renderer::setIndexBuffer", "index buffer is not a valid OpenGL buffer");
		
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ib->vboId);
		indexBufferFormat = TGen::OpenGL::TgenFormatToOpenGL(buffer->getVertexStructure().getElementDataType(0));
		lastIb = buffer;
	}
}

void TGen::OpenGL::Renderer::setTexture(int unit, TGen::Texture * texture) {
	glActiveTexture(GL_TEXTURE0 + unit);
	
	if (!texture) {
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}
	
	glBindTexture(GL_TEXTURE_2D, static_cast<TGen::OpenGL::Texture *>(texture)->texId);
}

void TGen::OpenGL::Renderer::drawPrimitive(TGen::PrimitiveType type, uint startVertex, uint vertexCount) {
	GLenum fixedPrimitive = TGen::OpenGL::TgenPrimitiveToOpenGL(type);	
	
	glDrawArrays(fixedPrimitive, startVertex, vertexCount);
}

void TGen::OpenGL::Renderer::drawIndexedPrimitive(TGen::PrimitiveType type, uint startIndex, uint indexCount) {
	GLenum fixedPrimitive = TGen::OpenGL::TgenPrimitiveToOpenGL(type);
	
	glDrawRangeElements(fixedPrimitive, 0, indexCount, indexCount, indexBufferFormat, reinterpret_cast<const GLvoid *>(startIndex * TGen::FormatTypeSize(lastIb->getVertexStructure().getElementDataType(0))));	
}

TGen::FrameBuffer * TGen::OpenGL::Renderer::createFrameBuffer() {
	if (!caps.framebuffer)
		throw TGen::RuntimeException("OpenGL::Renderer::CreateFrameBuffer", "framebuffers not supported");
	
	GLuint fbo = 0;
	glGenFramebuffersEXT(1, &fbo);
	
	DEBUG_PRINT("[opengl]: created framebuffer " << fbo);
	
	return new TGen::OpenGL::FrameBuffer(fbo);
}

TGen::Shader * TGen::OpenGL::Renderer::createVertexShader(const char * code) {
	if (!caps.vertexShader)
		throw TGen::RuntimeException("OpenGL::Renderer::CreateVertexShader", "vertex shaders not supported");
	
	return createShader(code, 0);
}

TGen::Shader * TGen::OpenGL::Renderer::createFragmentShader(const char * code) {
	if (!caps.fragmentShader)
		throw TGen::RuntimeException("OpenGL::Renderer::CreateFragmentShader", "fragment shaders not supported");

	return createShader(code, 1);
}

TGen::Shader * TGen::OpenGL::Renderer::createGeometryShader(const char * code) {
	if (!caps.geometryShader)
		throw TGen::RuntimeException("OpenGL::Renderer::CreateGeometryShader", "geometry shaders not supported");
		
	return createShader(code, 2);
}

TGen::Shader * TGen::OpenGL::Renderer::createShader(const char * code, int type) {
	GLuint newShader = 0;
	
	if (type == 0)
		newShader = glCreateShader(GL_VERTEX_SHADER);	
	else if (type == 1)
		newShader = glCreateShader(GL_FRAGMENT_SHADER);
	else
		newShader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
	
	glShaderSource(newShader, 1, const_cast<const GLchar **>(&code), NULL);
	glCompileShader(newShader);
	
	GLint status = 0, infoLogLength = 0;
	GLsizei charsWritten = 0;
	char * infoLog = NULL;
	std::string infoLogString;
	
	glGetShaderiv(newShader, GL_COMPILE_STATUS, &status);
	glGetShaderiv(newShader, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	if (infoLogLength > 0) {
		infoLog = static_cast<char *>(malloc(infoLogLength + 1));
		glGetShaderInfoLog(newShader, infoLogLength, &charsWritten, infoLog);
		infoLogString = infoLog;
		free(infoLog);		
	}
	
	if (status != GL_TRUE) {
		if (infoLogString.empty())
			infoLogString = "Unknown error";
		
		//std::cout << "[opengl]: failed to compile fragment shader: " << std::endl << infoLogString << std::endl;
		glDeleteShader(newShader);
		newShader = 0;
		
		if (type == 0)
			throw TGen::RuntimeException("OpenGL::Renderer::CreateVertexShader", "failed to compile shader: \"" + infoLogString + "\"");			
		else
			throw TGen::RuntimeException("OpenGL::Renderer::CreateFragmentShader", "failed to compile shader: \"" + infoLogString + "\"");
	}
	
	if (type == 0)
		DEBUG_PRINT("[opengl]: created vertex shader " << newShader << ": " << infoLogString);		
	else if (type == 1)
		DEBUG_PRINT("[opengl]: created fragment shader " << newShader << ": " << infoLogString);
	
	return new TGen::OpenGL::Shader(*this, newShader);		
}

TGen::ShaderProgram * TGen::OpenGL::Renderer::createShaderProgram() {
	GLuint newProgram = glCreateProgram();
	
	return new TGen::OpenGL::ShaderProgram(*this, newProgram);	
}

TGen::ShaderProgram * TGen::OpenGL::Renderer::createShaderProgram(const char * code) {
	TGen::ShaderProgram * program = createShaderProgram();
	char * buffer = static_cast<char *>(malloc(strlen(code) + 1));
	strcpy(buffer, code);
	program->parseShaders(*this, buffer);
	free(buffer);
	
	return program;
}


void TGen::OpenGL::Renderer::setRenderTarget(TGen::FrameBuffer * buffer) {
	TGen::OpenGL::FrameBuffer * fixedBuffer = static_cast<TGen::OpenGL::FrameBuffer *>(buffer);
		
	if (buffer) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fixedBuffer->getInternalID());		
		fixedBuffer->setupDrawBuffers();
	}
	else {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}

void TGen::OpenGL::Renderer::setShaderProgram(TGen::ShaderProgram * program) {
	if (!program) {
		glUseProgram(0);
		return;
	}
	
	TGen::OpenGL::ShaderProgram * fixedProgram = static_cast<TGen::OpenGL::ShaderProgram *>(program);
	glUseProgram(fixedProgram->getInternalID());
}

// TODO: kolla så alla texturer som attachas på fb har samma storlek

void TGen::OpenGL::Renderer::applyVertexStructure(const TGen::VertexStructure & vertstruct) {
	TGen::VertexElement element;
	uint pos = 0, lastSize = 0;
	uint stride = vertstruct.getSize();
	
	int elementCount = vertstruct.getElementCount();
	
	hasColorArray = false;
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	
	for (int i = 0; i < caps.maxTextureUnits; ++i) {
		glClientActiveTexture(GL_TEXTURE0 + i);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	
	for (int i = 0; i < elementCount; ++i) {
		vertstruct.getElement(i, element);
		GLenum fixedType = 0;
		int dataTypeSize = 0;
		
		if (!element.shared)
			pos += lastSize;
		
		fixedType = TGen::OpenGL::TgenFormatToOpenGL(element.dataType);
		dataTypeSize = TGen::FormatTypeSize(element.dataType);
		
		
		switch (element.type) {
			case TGen::VertexElementCoord:
				//std::cout << "vertex pointer count: " << int(element.count) << " type: " << fixedType << " stride: " << stride << " pos: " << pos << std::endl;
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(element.count, fixedType, stride, reinterpret_cast<GLvoid *>(pos));
				break;
				
			case TGen::VertexElementTexCoord:
				//std::cout << "texcoord pointer count: " << int(element.count) << " type: " << fixedType << " unit: " << int(element.unit) << " stride: " << stride << " pos: " << pos << std::endl;
				
				glClientActiveTexture(GL_TEXTURE0 + element.unit);	// NOTE
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(element.count, fixedType, stride, reinterpret_cast<GLvoid *>(pos));
				break;
				
			case TGen::VertexElementNormal:
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(fixedType, stride, reinterpret_cast<GLvoid *>(pos));
				break;
				
			case TGen::VertexElementColor:
				//std::cout << "color pointer count: " << int(element.count) << " type: " << fixedType << " stride: " << stride << " pos: " << pos << std::endl;
				//if (colorFromVertex) {
					glEnableClientState(GL_COLOR_ARRAY);
					glColorPointer(element.count, fixedType, stride, reinterpret_cast<GLvoid *>(pos));
				//}
				hasColorArray = true;
				
				break;
				
			case TGen::VertexElementColorIndex:
				if (colorFromVertex) {
					glEnableClientState(GL_INDEX_ARRAY);
					glIndexPointer(fixedType, stride, reinterpret_cast<GLvoid *>(pos));
				}
				break;
				
			case TGen::VertexElementEdgeFlag:
				glEnableClientState(GL_EDGE_FLAG_ARRAY);
				glEdgeFlagPointer(stride, reinterpret_cast<GLvoid *>(pos));
				break;
				
			case TGen::VertexElementAttribute:
				if (!element.bound)
					throw TGen::RuntimeException("OpenGL::Renderer::ApplyVertexStructure", "trying to use a vertex structure with unbound vertex attribute unit");
				
				//std::cout << "ddddddBOUND UNIT: " << int(element.boundUnit) << std::endl;
								
				glVertexAttribPointer(element.boundValue, element.count, fixedType, (element.normalize ? GL_TRUE : GL_FALSE), stride, reinterpret_cast<GLvoid *>(pos));
				glEnableVertexAttribArray(element.boundValue);
				
				break;
				
			default:
				throw TGen::NotImplemented("OpenGL::Renderer::ApplyVertexStructure", "vertex structure element type invalid");					
		}
		
		if (!element.shared)
			lastSize = dataTypeSize * element.count;
	}	
}

// TODO: kolla om texturen är för stor med PROXY_TEX
void TGen::OpenGL::Renderer::setClipPlane(int id, const TGen::Plane3 & plane) {
	glEnable(GL_CLIP_PLANE0 + id);
	
	GLdouble eq[4] = {plane.normal.x, plane.normal.y, plane.normal.z, plane.distance};
	glClipPlane(GL_CLIP_PLANE0 + id, eq);
}

void TGen::OpenGL::Renderer::disableClipPlane(int id) {
	glDisable(GL_CLIP_PLANE0 + id);
}

void TGen::OpenGL::Renderer::setRenderContext(const TGen::RenderContext & context, TGen::Texture ** textureTypes) {
	if (context.depthWrite)
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);
	
	setShaderProgram(context.shader);
	setColor(context.frontColor);	
	setDepthFunc(context.depthFunc);
	
	TGen::RenderContext::TextureList::const_iterator iter = context.textureUnits.begin();
	for (; iter != context.textureUnits.end(); ++iter) {
		glActiveTexture(GL_TEXTURE0 + (*iter)->unit);
		
		if ((*iter)->textureType == 0)
			setTexture((*iter)->unit, (*iter)->texture);
		else if ((*iter)->textureType > 0)
			setTexture((*iter)->unit, textureTypes[(*iter)->textureType]);
		else
			setTexture((*iter)->unit, NULL);
		
		setTextureCoordGen((*iter)->genU, (*iter)->genV);
		
		if ((*iter)->transformed) {
			setTransform(TGen::TransformTexture, (*iter)->transform);
		}
		else {
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
		}
	}
	
	glBlendFunc(TGen::OpenGL::TgenBlendFuncToOpenGL(context.blendSrc), TGen::OpenGL::TgenBlendFuncToOpenGL(context.blendDst));
	glEnable(GL_CULL_FACE);
	
	if (context.front == TGen::PolygonFaceCull && context.back == TGen::PolygonFaceCull)
		glCullFace(GL_FRONT_AND_BACK);
	else if (context.front == TGen::PolygonFaceCull)
		glCullFace(GL_FRONT);
	else if (context.back == TGen::PolygonFaceCull)
		glCullFace(GL_BACK);
	else if (context.back != TGen::PolygonFaceCull && context.front != TGen::PolygonFaceCull)
		glDisable(GL_CULL_FACE);
	
	switch (context.front) {
		case TGen::PolygonFaceFill:
			if (context.back == TGen::PolygonFaceFill)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);				
			else
				glPolygonMode(GL_FRONT, GL_FILL);
			break;
			
		case TGen::PolygonFaceLines:
			if (context.back == TGen::PolygonFaceLines)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);				
			else
				glPolygonMode(GL_FRONT, GL_LINE);
			break;
			
		case TGen::PolygonFacePoints:
			if (context.back == TGen::PolygonFacePoints)
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);				
			else
				glPolygonMode(GL_FRONT, GL_POINT);
			break;
			
		case TGen::PolygonFaceCull:
			break;
			
		default:
			throw TGen::NotImplemented("OpenGL::Renderer::setRenderContext", "front polygon mode not supported");						
	}
	
	switch (context.back) {
		case TGen::PolygonFaceFill:
			if (context.front != TGen::PolygonFaceFill)
				glPolygonMode(GL_BACK, GL_FILL);
			break;
			
		case TGen::PolygonFaceLines:
			if (context.front != TGen::PolygonFaceLines)
				glPolygonMode(GL_BACK, GL_LINE);
			break;
			
		case TGen::PolygonFacePoints:
			if (context.front != TGen::PolygonFacePoints)
				glPolygonMode(GL_BACK, GL_POINT);
			break;
			
		case TGen::PolygonFaceCull:
			break;
			
		default:
			throw TGen::NotImplemented("OpenGL::Renderer::setRenderContext", "back polygon mode not supported");						
	}
	
	// hm... setRenderContext s�tts per pass, applyVertexStructure s�tts preRender. s� colorFromVertex g�r inget...
	colorFromVertex = context.colorFromVertex;
	
	if (colorFromVertex && hasColorArray)
		glEnableClientState(GL_COLOR_ARRAY);
	else
		glDisableClientState(GL_COLOR_ARRAY);
}


void TGen::OpenGL::Renderer::setTextureCoordGen(TGen::TextureCoordGen genU, TGen::TextureCoordGen genV) {
	if (genU == TGen::TextureCoordGenBase) {
		glDisable(GL_TEXTURE_GEN_S);
	}
	else {
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, TGen::OpenGL::TgenTextureCoordGenToOpenGL(genU));			
		glEnable(GL_TEXTURE_GEN_S);
	}
	
	if (genV == TGen::TextureCoordGenBase) {
		glDisable(GL_TEXTURE_GEN_T);
	}
	else {
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, TGen::OpenGL::TgenTextureCoordGenToOpenGL(genV));		
		glEnable(GL_TEXTURE_GEN_T);
	}
}


void TGen::OpenGL::Renderer::setDepthFunc(TGen::CompareFunc compare) {
	glDepthFunc(TGen::OpenGL::TgenCompareFuncToOpenGL(compare));
}

