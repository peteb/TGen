/*
 *  indexbuffer.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "indexbuffer.h"

TGen::IndexBuffer::IndexBuffer(TGen::Renderer & creator, uint size, ushort usage) 
	: creator(creator)
	, size(size)
	, usage(usage)
{

}

TGen::IndexBuffer::~IndexBuffer() {}

uint TGen::IndexBuffer::getSize() const {
	return size;
}

