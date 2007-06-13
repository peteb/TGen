/*
 *  vertexbuffer.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vertexbuffer.h"

TGen::VertexBuffer::VertexBuffer(TGen::Renderer & creator, uint size, ushort usage) : creator(creator), size(size), stride(0), usage(usage) {}
TGen::VertexBuffer::~VertexBuffer() {}

uint TGen::VertexBuffer::getSize() const {
	return size;
}

