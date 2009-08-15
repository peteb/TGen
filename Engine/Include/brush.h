/*
 *  brush.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/15/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_BRUSH_H
#define _TGEN_ENGINE_BRUSH_H

#include <vector>

namespace TGen {
	class Vector3;
	class Plane3;
	
	namespace Engine {
		std::vector<TGen::Vector3> TriangulateBrush(const std::vector<TGen::Plane3> & brush);
		std::vector<TGen::Vector3> TriangulateBrushside(const std::vector<TGen::Plane3> & brush, int side);
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_BRUSH_H

