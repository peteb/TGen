/*
 *  vertexsource.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 12/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vertexsource.h"

TGen::VertexData::VertexData(TGen::VertexDataSource & creator, uint size, ushort usage) 
	: creator(creator)
	, size(size)
	, usage(usage)
{

}

TGen::VertexData::~VertexData() {
	creator.removeVertexData(this);
}

uint TGen::VertexData::getSize() const {
	return size;
}

