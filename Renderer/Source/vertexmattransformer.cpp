/*
 *  vertexmattransformer.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 6/16/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "vertexmattransformer.h"

TGen::VertexMatrixTransformer::VertexMatrixTransformer(const TGen::Matrix4x4 & matrix)
	: matrix(matrix)
{
}

void TGen::VertexMatrixTransformer::transform(TGen::Vector3 & vector) const {
	vector = matrix * vector;
}

void TGen::VertexMatrixTransformer::transform(TGen::Plane3 & plane) const {
	//vector = matrix * vector;
	plane.normal = matrix * plane.normal;
	//plane.distance = (matrix * (plane.normal * plane.distance)).getMagnitude();	// TODO: dunno if this works
}

TGen::VertexMatrixTransformer * TGen::VertexMatrixTransformer::clone() const {
	return new TGen::VertexMatrixTransformer(matrix);
}

