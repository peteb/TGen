/*
 *  vertexswapper.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "vertexswapper.h"
#include <tgen_core.h>
#include <tgen_math.h>

TGen::VertexSwapper::VertexSwapper(const TGen::VertexSwapper::SwapAxes & ax1, const TGen::VertexSwapper::SwapAxes & ax2) 
	: ax1(ax1)
	, ax2(ax2)
{
	if (ax1 == ax2)
		throw TGen::RuntimeException("VertexSwapper::VertexSwapper", "can't swap component with itself");
	
}

void TGen::VertexSwapper::transform(TGen::Vector3 & vector) const {
	switch (ax1) {
		case X_AXIS:
			switch (ax2) {
				case Y_AXIS:
					std::swap(vector.x, vector.y);
					break;
					
				case Z_AXIS:
					std::swap(vector.x, vector.z);
					break;
			}
			
			break;
			
		case Y_AXIS:
			switch (ax2) {
				case X_AXIS:
					std::swap(vector.y, vector.x);
					break;
					
				case Z_AXIS:
					std::swap(vector.y, vector.z);
					break;
			}
			
			break;
			
		case Z_AXIS:
			switch (ax2) {
				case X_AXIS:
					std::swap(vector.z, vector.x);
					break;
					
				case Y_AXIS:
					std::swap(vector.z, vector.y);
					break;
			}
			
			break;
	}
	
}

TGen::VertexSwapper * TGen::VertexSwapper::clone() const {
	return new TGen::VertexSwapper(ax1, ax2);
}

