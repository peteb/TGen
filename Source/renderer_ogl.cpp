/*
 *  renderer_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderer_ogl.h"
#include "platform.h"
#include "vertexstructure.h"
#include "vertexbuffer_ogl.h"
#include "error.h"

#ifdef _PLATFORM_OSX
	#include <OpenGL/OpenGL.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/GL.h>
#endif

#include <iostream>

TGen::OpenGL::Renderer::Renderer() {}
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
	
	if (usage == TGen::UsageDynamic)
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, NULL, GL_STREAM_DRAW_ARB);
	else
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, NULL, GL_STATIC_DRAW_ARB);
	
	return new TGen::OpenGL::VertexBuffer(*this, vertstruct, size, usage, newVBO);
}

void TGen::OpenGL::Renderer::setVertexBuffer(TGen::VertexBuffer * buffer) {
	if (!buffer) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);		
	}
	else {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<TGen::OpenGL::VertexBuffer *>(buffer)->vboId);

		TGen::VertexElement element;
		uint pos = 0;
		uint stride = buffer->getVertexStructure().getSize();
		
		int elementCount = buffer->getVertexStructure().getElementCount();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_EDGE_FLAG_ARRAY);
		
		for (int i = 0; i < elementCount; ++i) {
			buffer->getVertexStructure().getElement(i, element);
			
			/*enum VertexElementType {
				CoordElement = 1,
				NormalElement,
				ColorElement,
				IndexElement,
				EdgeFlagElement,
				TexCoordElement,
			};
			
			enum VertexElementDataType {
				TypeFloat = 1,
				TypeDouble,
				TypeInt,
				TypeShort,
			};*/
			
			GLenum fixedType = 0;
			int dataTypeSize = 0;
			
			switch (element.dataType) {
				case TGen::TypeFloat:
					fixedType = GL_FLOAT;
					dataTypeSize = sizeof(float);
					break;
					
				case TGen::TypeDouble:
					fixedType = GL_DOUBLE;
					dataTypeSize = sizeof(double);
					break;
					
				case TGen::TypeInt:
					fixedType = GL_INT;
					dataTypeSize = sizeof(int);
					break;
					
				case TGen::TypeShort:
					fixedType = GL_SHORT;
					dataTypeSize = sizeof(short);
					break;
					
				default:
					throw TGen::NotImplemented("OpenGL::Renderer::setVertexBuffer", "vertex structure datatype invalid");				
			}
			
		
			switch (element.type) {
				case TGen::CoordElement:
					std::cout << "vertex pointer count: " << int(element.count) << " type: " << fixedType << " stride: " << stride << " pos: " << pos << std::endl;
					glVertexPointer(element.count, fixedType, stride, reinterpret_cast<GLvoid *>(pos));
					glEnableClientState(GL_VERTEX_ARRAY);
					break;
					
				case TGen::TexCoordElement:
					std::cout << "texcoord pointer count: " << int(element.count) << " type: " << fixedType << " unit: " << int(element.unit) << " stride: " << stride << " pos: " << pos << std::endl;
					
					glActiveTexture(GL_TEXTURE0 + element.unit);
					glTexCoordPointer(element.count, fixedType, stride, reinterpret_cast<GLvoid *>(pos));
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					break;
					
				case TGen::NormalElement:
					glNormalPointer(fixedType, stride, reinterpret_cast<GLvoid *>(pos));
					glEnableClientState(GL_NORMAL_ARRAY);
					break;
					
				case TGen::ColorElement:
					std::cout << "color pointer count: " << int(element.count) << " type: " << fixedType << " stride: " << stride << " pos: " << pos << std::endl;
					glColorPointer(element.count, fixedType, stride, reinterpret_cast<GLvoid *>(pos));
					glEnableClientState(GL_COLOR_ARRAY);
					break;
					
				case TGen::IndexElement:
					glIndexPointer(fixedType, stride, reinterpret_cast<GLvoid *>(pos));
					glEnableClientState(GL_INDEX_ARRAY);
					break;
					
				case TGen::EdgeFlagElement:
					glEdgeFlagPointer(stride, reinterpret_cast<GLvoid *>(pos));
					glEnableClientState(GL_EDGE_FLAG_ARRAY);
					break;
					
				default:
					throw TGen::NotImplemented("OpenGL::Renderer::setVertexBuffer", "vertex structure element type invalid");					
			}
			
			pos += dataTypeSize * element.count;
		}
	}
}

void TGen::OpenGL::Renderer::DrawPrimitive(TGen::PrimitiveType type, uint startVertex, uint vertexCount) {
	GLenum fixedPrimitive = 0;
	
	switch (type) {
		case TGen::PrimitiveQuads:
			fixedPrimitive = GL_QUADS;
			break;
			
		case TGen::PrimitiveTriangles:
			fixedPrimitive = GL_TRIANGLES;
			break;
			
		case TGen::PrimitiveLines:
			fixedPrimitive = GL_LINES;
			break;
			
		default:
			throw TGen::NotImplemented("OpenGL::Renderer::DrawPrimitive", "primitive type is not supported");
	}
	
	glDrawArrays(fixedPrimitive, startVertex, vertexCount);
}
