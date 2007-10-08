/*
 *  coordset3.h
 *  TGen Math
 *
 *  Created by Peter Backman on 9/10/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_COORDSET3_H
#define _TGEN_COORDSET3_H

#include "vector3.h"
#include <vector>
#include "AABB.h"

namespace TGen {
	class Matrix4x4;
	
	class CoordSet3 {
	public:	
		
		void addCoord(const TGen::Vector3 & coord);
		int size() const;
		
		const TGen::Vector3 & operator [] (int index) const;
		CoordSet3 & operator *= (const TGen::Matrix4x4 & mat);
		CoordSet3 & operator += (const TGen::CoordSet3 & set);
		
		bool intersects(const CoordSet3 & otherPolygon) const;
		void projectOnAxis(const TGen::Vector3 & axis, scalar & min, scalar & max) const;
		
		TGen::AABB getBoundingBox() const;
		TGen::Vector3 getNormal(int index) const;
		TGen::Vector3 getEdge(int index) const;
		
		TGen::Vector3 getMin() const;
		TGen::Vector3 getMax() const;
		
		operator std::string() const;
		
	private:
		
		typedef std::vector<TGen::Vector3> CoordSet;
		CoordSet coords;
	};
	
	
	
} // !TGen

#endif // !_TGEN_COORDSET3_H

