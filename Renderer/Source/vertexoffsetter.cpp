/*
 *  vertexoffsetter.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 5/6/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "vertexoffsetter.h"
#include <tgen_math.h>

TGen::VertexOffsetter::VertexOffsetter(const TGen::Vector3 & offset) 
	: offset(offset)
{
}

void TGen::VertexOffsetter::transform(TGen::Vector3 & vector) const {
	vector += offset;
}

void TGen::VertexOffsetter::transform(TGen::Plane3 & plane) const {
	// TODO: hur offsettar man ett plan med en punkt?
}

TGen::VertexOffsetter * TGen::VertexOffsetter::clone() const {
	return new TGen::VertexOffsetter(offset);
}
