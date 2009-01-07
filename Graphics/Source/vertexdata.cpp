/*
 *  vertexdata.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "vertexdata.h"
#include "vertexsource.h"

using TGen::uint;

TGen::VertexData::VertexData(TGen::VertexDataSource & creator, uint size, ushort usage, void * writeOffset, uint readOffset) 
	: creator(creator)
	, size(size)
	, usage(usage)
	, writeOffset(writeOffset)
	, readOffset(readOffset)
{
}

TGen::VertexData::~VertexData() {
	creator.removeVertexData(this);
}

uint TGen::VertexData::getSize() const {
	return size;
}

uint TGen::VertexData::getUsage() const {
	return usage;
}

uint TGen::VertexData::getSortKey() const {
	return reinterpret_cast<uint>(this);
}

uint TGen::VertexData::getReadOffset() const {
	return readOffset;
}

TGen::VertexData * TGen::VertexData::operator * () {
	return this;
}
