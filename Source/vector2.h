/*
 *  vector2.h
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_VECTOR2_H
#define _TGEN_VECTOR2_H

#include <string>

namespace TGen {
	class Vector2 {
	public:	
		Vector2(const TGen::Vector2 & vector);
		Vector2(float x, float y);
		Vector2(float scalar = 0.0f);
		
		Vector2 & operator += (const TGen::Vector2 & vector);
		Vector2 & operator -= (const TGen::Vector2 & vector);
		Vector2 & operator = (const TGen::Vector2 & vector);
		Vector2 & operator *= (float scalar);
		
		Vector2 operator + (const TGen::Vector2 & vector) const;
		Vector2 operator - (const TGen::Vector2 & vector) const;
		Vector2 operator * (float scalar) const;
		Vector2 operator / (float scalar) const;
		Vector2 operator - () const;
		
		operator std::string() const;
		
		float getMagnitude() const;
		Vector2 getNormalized() const;
		Vector2 & Normalize();
		
		Vector2 getPerpendicular() const;
		static float DotProduct(const TGen::Vector2 & v1, const TGen::Vector2 & v2);
		static TGen::Vector2 CrossProduct(const TGen::Vector2 & v1, const TGen::Vector2 & v2);
		
		float x, y;
	}; // !Vector2
	
} // !TGen

#endif // !_TGEN_VECTOR2_H
