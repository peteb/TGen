/*
 *  coordset2.h
 *  TGen Math
 *
 *  Created by Peter Backman on 9/10/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_COORDSET2_H
#define _TGEN_COORDSET2_H

#include "vector2.h"
#include <vector>
#include "rectangle.h"

namespace TGen {
	class Matrix4x4;
	
	class CoordSet2 {
	public:	
		void addCoord(const TGen::Vector2 & coord);
		int size() const;
		
		const TGen::Vector2 & operator [] (int index) const;
		CoordSet2 & operator *= (const TGen::Matrix4x4 & mat);
		CoordSet2 & operator += (const TGen::CoordSet2 & set);
		
		bool intersects(const CoordSet2 & otherPolygon) const;
		void projectOnAxis(const TGen::Vector2 & axis, scalar & min, scalar & max) const;
		
		//TGen::AABB getBoundingBox() const;
		TGen::Rectangle getBoundingBox() const;
		TGen::Vector2 getNormal(int index) const;
		TGen::Vector2 getEdge(int index) const;
		
		TGen::Vector2 getMin() const;
		TGen::Vector2 getMax() const;
		
		operator std::string() const;
		
	private:
		
		typedef std::vector<TGen::Vector2> CoordSet;
		CoordSet coords;
	};
	
	
	
} // !TGen

#endif // !_TGEN_COORDSET3_H

