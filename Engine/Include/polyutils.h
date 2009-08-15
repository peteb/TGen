/*
 *  polyutils.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/15/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_POLYUTILS_H
#define _TGEN_ENGINE_POLYUTILS_H

#include <vector>

namespace TGen {
	class Vector3;
	class Plane3;
	
	namespace Engine {
		std::vector<TGen::Vector3> TriangulateConvexPolygon(const std::vector<TGen::Vector3> & polygon);
		std::vector<TGen::Vector3> RewindVertices(const std::vector<TGen::Vector3> & vertices, const TGen::Vector3 & normal);
		std::pair<TGen::Vector3, TGen::Vector3> CalculateBoundingBox(const std::vector<TGen::Vector3> & vertices);
		TGen::Vector3 CalculateAveragePoint(const std::vector<TGen::Vector3> & vertices);
		TGen::Vector3 CalculateMidpoint(const std::vector<TGen::Vector3> & vertices);
		bool PointInsidePlanes(const TGen::Vector3 & point, const std::vector<TGen::Plane3> & planes);
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_POLYUTILS_H

