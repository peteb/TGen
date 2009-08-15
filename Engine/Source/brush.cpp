/*
 *  brush.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/15/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "brush.h"
#include "polyutils.h"

#include <tgen_math.h>

std::vector<TGen::Vector3> TGen::Engine::TriangulateBrush(const std::vector<TGen::Plane3> & brush) {
	std::vector<TGen::Vector3> triangles;
	
	for (int i = 0; i < brush.size(); ++i) {
		std::vector<TGen::Vector3> sideTris = TriangulateBrushside(brush, i);
		
		for (int i = 0; i < sideTris.size(); ++i)
			triangles.push_back(sideTris[i]);	
	}
	
	return triangles;
}

std::vector<TGen::Vector3> TGen::Engine::TriangulateBrushside(const std::vector<TGen::Plane3> & brush, int side) {
	const TGen::Plane3 & brushPlane = brush[side];
	std::vector<TGen::Vector3> vertices;
	
	for (int a = 0; a < brush.size(); ++a) {
		if (a != side) {
			const TGen::Plane3 & secondPlane = brush[a];
			
			for (int b = 0; b < brush.size(); ++b) {
				if (b != a) {
					const TGen::Plane3 & thirdPlane = brush[b];
					bool foundIntersection = false;
					
					TGen::Vector3 intersectionPoint = TGen::Plane3::Intersect3Planes(brushPlane, secondPlane, thirdPlane, foundIntersection);
					
					if (foundIntersection) {
						if (TGen::Engine::PointInsidePlanes(intersectionPoint, brush)) {
							if (std::find(vertices.begin(), vertices.end(), intersectionPoint) == vertices.end())
								vertices.push_back(intersectionPoint);
						}
					}
					
				}
			}
			
		}			
	}
	
	std::vector<TGen::Vector3> triangles;

	if (!vertices.empty()) {
		std::vector<TGen::Vector3> windedVertices = TGen::Engine::RewindVertices(vertices, -brushPlane.normal);
		std::vector<TGen::Vector3> faceTriangles = TGen::Engine::TriangulateConvexPolygon(windedVertices);
		
		for (int i = 0; i < faceTriangles.size(); ++i)
			triangles.push_back(faceTriangles[i]);
	}
	else {
		std::cout << "*** WARNING: brushside triangulator, zero vertices generated" << std::endl;
	}
	
	
	return triangles;
}

