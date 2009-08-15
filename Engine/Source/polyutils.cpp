/*
 *  polyutils.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/15/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "polyutils.h"
#include <tgen_math.h>
#include <map>

// creates a triangle list
std::vector<TGen::Vector3> TGen::Engine::TriangulateConvexPolygon(const std::vector<TGen::Vector3> & polygon) {
	std::vector<TGen::Vector3> result;
	
	result.reserve((polygon.size() + 2) / 3);
	
	std::list<TGen::Vector3> vertices;
	for (int i = 0; i < polygon.size(); ++i)
		vertices.push_back(polygon[i]);
	
	
	while (vertices.size() > 2) {
		result.push_back(vertices.back());
		result.push_back(vertices.front());
		
		std::list<TGen::Vector3>::iterator iter = vertices.begin();
		iter++;
		
		result.push_back(*iter);
		
		vertices.erase(vertices.begin());
	}
	
	return result;
}


// winds a vertex soup 
std::vector<TGen::Vector3> TGen::Engine::RewindVertices(const std::vector<TGen::Vector3> & vertices, const TGen::Vector3 & normal) {
	TGen::Vector3 midpoint = CalculateAveragePoint(vertices);
	
	TGen::Vector3 diff1 = vertices.at(0) - midpoint;
	diff1.normalize();
	
	typedef std::map<float, TGen::Vector3> AngleMap;
	AngleMap sorted;
	
	sorted[0.0f] = vertices.at(0);
	
	for (int i = 1; i < vertices.size(); ++i) {
		TGen::Vector3 diff2 = vertices[i] - midpoint;
		diff2.normalize();
		
		float angle = TGen::Vector3::SignedAngle(diff1, diff2, normal);
		
		sorted[angle] = vertices[i];
	}
	
	std::vector<TGen::Vector3> result;
	
	for (AngleMap::iterator iter = sorted.begin(); iter != sorted.end(); ++iter) {
		result.push_back(iter->second);
	}
	
	return result;
}

std::pair<TGen::Vector3, TGen::Vector3> TGen::Engine::CalculateBoundingBox(const std::vector<TGen::Vector3> & vertices) {
	TGen::Vector3 min, max;
	min = max = vertices.at(0);
	
	for (int i = 1; i < vertices.size(); ++i) {
		const TGen::Vector3 & point = vertices[i];
		
		min = TGen::Vector3::Min(min, point);
		max = TGen::Vector3::Max(max, point);
	}
	
	return std::make_pair(min, max);
}

TGen::Vector3 TGen::Engine::CalculateAveragePoint(const std::vector<TGen::Vector3> & vertices) {
	TGen::Vector3 sum = vertices.at(0);
	 
	for (int i = 1; i < vertices.size(); ++i)
		sum = sum + vertices[i];
	 
	return sum / vertices.size();
}

TGen::Vector3 TGen::Engine::CalculateMidpoint(const std::vector<TGen::Vector3> & vertices) {
	std::pair<TGen::Vector3, TGen::Vector3> bounds = CalculateBoundingBox(vertices);
	return TGen::Interpolate(bounds.first, bounds.second, 0.5);
}

bool TGen::Engine::PointInsidePlanes(const TGen::Vector3 & point, const std::vector<TGen::Plane3> & planes) {
	const float epsilon = 0.00001f;
	
	for (int i = 0; i < planes.size(); ++i) {
		if (planes[i].getDistanceTo(point) >= epsilon)
			return false;
	}
	
	return true;
}
