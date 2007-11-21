/*
 *  rectangle.h
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RECTANGLE_H
#define _TGEN_RECTANGLE_H

#include "vector2.h"
#include <string>

namespace TGen {
	class Rectangle {
	public:	
		Rectangle();
		Rectangle(const TGen::Rectangle & rectangle);
		Rectangle(const TGen::Vector2 & center, scalar width, scalar height);
		Rectangle(scalar width, scalar height);
		Rectangle(const TGen::Vector2 & min, const TGen::Vector2 & max);
		
		Rectangle operator / (scalar value) const;
		
		Rectangle & operator = (const TGen::Rectangle & rectangle);
		operator std::string() const;
		
		bool intersects(const TGen::Rectangle & rect) const;
		void calculate(const TGen::Vector2 * vectors, int vectorcount);
		
		static TGen::Rectangle FromUpperLeft(const TGen::Vector2 & topleft, scalar width, scalar height);
		TGen::Vector2 getMin() const;
		TGen::Vector2 getMax() const;
		
		Vector2 center;
		scalar width, height;
	};
	
} // !TGen

#endif // !_TGEN_RECTANGLE_H
