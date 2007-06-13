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
		
		Rectangle & operator = (const TGen::Rectangle & rectangle);
		operator std::string() const;
		
		static TGen::Rectangle FromUpperLeft(const TGen::Vector2 & topleft, scalar width, scalar height);
		TGen::Vector2 getUpperLeft() const;
		TGen::Vector2 getLowerRight() const;
		
		Vector2 center;
		scalar width, height;
	};
	
} // !TGen

#endif // !_TGEN_RECTANGLE_H
