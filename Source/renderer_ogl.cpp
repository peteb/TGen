/*
 *  renderer_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "prefix_ogl.h"
#include "renderer_ogl.h"
#include "vertexstructure.h"
#include "vertexbuffer_ogl.h"
#include "indexbuffer_ogl.h"
#include "error.h"
#include "types_converter_ogl.h"

#include <iostream>

TGen::OpenGL::Renderer::Renderer() {
	GLint viewportDims[2];

	glGetIntegerv(GL_MAX_TEXTURE_UNITS, reinterpret_cast<GLint *>(&caps.maxTextureUnits));
	glGetIntegerv(GL_MAX_LIGHTS, reinterpret_cast<GLint *>(&caps.maxActiveLights));
	glGetIntegerv(GL_MAX_CLIP_PLANES, reinterpret_cast<GLint *>(&caps.maxClipPlanes));
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, reinterpret_cast<GLint *>(&caps.maxIndexBufferIndicies));
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, reinterpret_cast<GLint *>(&caps.maxVertexBufferVertices));
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, reinterpret_cast<GLint *>(&caps.maxTextureSize));
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewportDims);
	
	caps.maxViewportSize = TGen::Rectangle(viewportDims[0], viewportDims[1]);
	/*
	 maxClipPlanes, maxIndexBufferIndicies, maxVertexBufferVertices, maxTextureSize;
	 
	 TGen::Rectangle maxViewportSize;
	 */

}

TGen::OpenGL::Renderer::~Renderer() {}

void TGen::OpenGL::Renderer::setClearColor(const TGen::Color & color) {
	TGen::Renderer::setClearColor(color);

	glClearColor(color.r, color.g, color.g, color.a);
}

void TGen::OpenGL::Renderer::setViewport(const TGen::Rectangle & viewport) {
	TGen::Vector2 upperLeft = viewport.getUpperLeft();
	
	glViewport(upperLeft.x, upperLeft.y, viewport.width, viewport.height);	// NOTE: ogl origo is lower-left corner
}

void TGen::OpenGL::Renderer::setColor(const TGen::Color & color) {
	glColor4f(color.r, color.g, color.b, color.a);
}

void TGen::OpenGL::Renderer::Clear(ushort buffers) {
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
	switch (mode) {
		case TransformProjection:
			glMatrixMode(GL_PROJECTION);
			break;
			
		case TransformWorldView:
			glMatrixMode(GL_MODELVIEW);
			break;
			
		case TransformTexture:
			glMatrixMode(GL_TEXTURE);
			break;
		
		default:
			throw TGen::NotImplemented("OpenGL::Renderer::setTransform", "transform mode invalid");
	}

	TGen::Renderer::setTransform(mode, transformation);
	
	float elements[16];
	transformation.get4x4(elements);
	
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

	glGetFloatv(fixedTransform, (GLfloat *)ret.elements);
	
	return ret;
}


TGen::VertexBuffer * TGen::OpenGL::Renderer::CreateVertexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage) {
	GLuint newVBO = 0;
	glGenBuffersARB(1, &newVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, newVBO);
	
	std::cout << "[opengl]: created vertex buffer " << newVBO << std::endl;
	
	GLenum fixedUsage = 0;
	
	if (usage & TGen::UsageStream) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_STREAM_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_STREAM_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_STREAM_READ_ARB;
	}
	else if (usage & TGen::UsageDynamic) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_DYNAMIC_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_DYNAMIC_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_DYNAMIC_READ_ARB;		
	}
	else if (usage & TGen::UsageStream) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_STREAM_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_STREAM_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_STREAM_READ_ARB;		
	}
	
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, NULL, fixedUsage);

	return new TGen::OpenGL::VertexBuffer(*this, vertstruct, size, fixedUsage, newVBO);
}

TGen::IndexBuffer * TGen::OpenGL::Renderer::CreateIndexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage) {
	GLuint newVBO = 0;
	glGenBuffersARB(1, &newVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, newVBO);
	
	std::cout << "[opengl]: created index buffer " << newVBO << std::endl;
	
	GLenum fixedUsage = 0;
	
	if (usage & TGen::UsageStream) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_STREAM_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_STREAM_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_STREAM_READ_ARB;
	}
	else if (usage & TGen::UsageDynamic) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_DYNAMIC_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_DYNAMIC_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_DYNAMIC_READ_ARB;		
	}
	else if (usage & TGen::UsageStream) {
		if (usage & TGen::UsageDraw)
			fixedUsage = GL_STREAM_DRAW_ARB;
		else if (usage & TGen::UsageCopy)
			fixedUsage = GL_STREAM_COPY_ARB;
		else if (usage & TGen::UsageRead)
			fixedUsage = GL_STREAM_READ_ARB;		
	}
	
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, NULL, fixedUsage);
	
	return new TGen::OpenGL::IndexBuffer(*this, vertstruct, size, fixedUsage, newVBO);	
}

// TODO: texturer, compression om möjligt

void TGen::OpenGL::Renderer::setVertexBuffer(TGen::VertexBuffer * buffer) {
	if (!buffer) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);		
	}
	else {
		TGen::OpenGL::VertexBuffer * vb = static_cast<TGen::OpenGL::VertexBuffer *>(buffer);
		if (!vb) // NOTE: this is never thrown, static_cast!
			throw TGen::RuntimeException("OpenGL::Renderer::setVertexBuffer", "vertex buffer is not a valid OpenGL buffer");
		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vb->vboId);

		ApplyVertexStructure(buffer->getVertexStructure());
	}
}

void TGen::OpenGL::Renderer::setIndexBuffer(TGen::IndexBuffer * buffer) {	// You HAVE to call these before DrawPrimitive*, no checking is done
	if (!buffer) {
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}
	else {
		TGen::OpenGL::IndexBuffer * ib = static_cast<TGen::OpenGL::IndexBuffer *>(buffer);
		if (!ib) // NOTE: this is never thrown, static_cast!
			throw TGen::RuntimeException("OpenGL::Renderer::setIndexBuffer", "index buffer is not a valid OpenGL buffer");
		
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ib->vboId);

		indexBufferFormat = TGen::OpenGL::TgenFormatToOpenGL(buffer->getVertexStructure().getElementDataType(0));
	}
}

void TGen::OpenGL::Renderer::DrawPrimitive(TGen::PrimitiveType type, uint startVertex, uint vertexCount) {
	GLenum fixedPrimitive = TGen::OpenGL::TgenPrimitiveToOpenGL(type);	
	
	glDrawArrays(fixedPrimitive, startVertex, vertexCount);
}

void TGen::OpenGL::Renderer::DrawIndexedPrimitive(TGen::PrimitiveType type, uint startIndex, uint indexCount) {
	GLenum fixedPrimitive = TGen::OpenGL::TgenPrimitiveToOpenGL(type);
	
	glDrawRangeElements(fixedPrimitive, startIndex, startIndex + indexCount, indexCount, indexBufferFormat, NULL);
}

void TGen::OpenGL::Renderer::ApplyVertexStructure(const TGen::VertexStructure & vertstruct) {
	TGen::VertexElement element;
	uint pos = 0, lastSize = 0;
	uint stride = vertstruct.getSize();
	
	int elementCount = vertstruct.getElementCount();
	
	// Disabling these should be pretty fast
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
			case TGen::CoordElement:
				//std::cout << "vertex pointer count: " << int(element.count) << " type: " << fixedType << " stride: " << stride << " pos: " << pos << std::endl;
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(element.count, fixedType, stride, reinterpret_cast<GLvoid *>(pos));
				break;
				
			case TGen::TexCoordElement:
				//std::cout << "texcoord pointer count: " << int(element.count) << " type: " << fixedType << " unit: " << int(element.unit) << " stride: " << stride << " pos: " << pos << std::endl;
				
				glClientActiveTexture(GL_TEXTURE0 + element.unit);	// NOTE
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(element.count, fixedType, stride, reinterpret_cast<GLvoid *>(pos));
				break;
				
			case TGen::NormalElement:
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(fixedType, stride, reinterpret_cast<GLvoid *>(pos));
				break;
				
			case TGen::ColorElement:
				//std::cout << "color pointer count: " << int(element.count) << " type: " << fixedType << " stride: " << stride << " pos: " << pos << std::endl;
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(element.count, fixedType, stride, reinterpret_cast<GLvoid *>(pos));
				break;
				
			case TGen::ColorIndexElement:
				glEnableClientState(GL_INDEX_ARRAY);
				glIndexPointer(fixedType, stride, reinterpret_cast<GLvoid *>(pos));
				break;
				
			case TGen::EdgeFlagElement:
				glEnableClientState(GL_EDGE_FLAG_ARRAY);
				glEdgeFlagPointer(stride, reinterpret_cast<GLvoid *>(pos));
				break;
				
			default:
				throw TGen::NotImplemented("OpenGL::Renderer::ApplyVertexStructure", "vertex structure element type invalid");					
		}
		
		if (!element.shared)
			lastSize = dataTypeSize * element.count;
	}	
}

// TODO: kolla om texturen är för stor med PROXY_TEX
