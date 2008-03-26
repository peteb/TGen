/*
 *  vertexscaler.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "vertexscaler.h"
#include <tgen_math.h>

TGen::VertexScaler::VertexScaler(scalar scale) 
	: scale(scale)
{
}

void TGen::VertexScaler::transform(TGen::Vector3 & vector) const {
	vector = vector * scale;
}

TGen::VertexScaler * TGen::VertexScaler::clone() const {
	return new TGen::VertexScaler(scale);
}
