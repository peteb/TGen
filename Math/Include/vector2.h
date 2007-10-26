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
#include "types.h"

namespace TGen {
	class Vector3;
	
	class Vector2 {
	public:	
		Vector2(const TGen::Vector2 & vector);
		Vector2(const TGen::Vector3 & vector);
		Vector2(scalar x, scalar y);
		Vector2(scalar scalar = 0.0f);
		
		Vector2 & operator += (const TGen::Vector2 & vector);
		Vector2 & operator -= (const TGen::Vector2 & vector);
		Vector2 & operator *= (const TGen::Vector2 & vector);
		Vector2 & operator = (const TGen::Vector2 & vector);
		Vector2 & operator *= (scalar scalar);
		
		Vector2 operator + (const TGen::Vector2 & vector) const;
		Vector2 operator - (const TGen::Vector2 & vector) const;
		Vector2 operator * (const TGen::Vector2 & vector) const;		
		Vector2 operator * (scalar _scalar) const;
		Vector2 operator / (scalar _scalar) const;
		Vector2 operator - () const;
		
		operator std::string() const;
		
		scalar getMagnitude() const;
		Vector2 getNormalized() const;
		Vector2 & normalize();
		Vector2 getPerpendicular() const;
		
		static scalar DotProduct(const TGen::Vector2 & v1, const TGen::Vector2 & v2);
		static TGen::Vector2 CrossProduct(const TGen::Vector2 & v1, const TGen::Vector2 & v2);
		static TGen::Vector2 Identity;
		
		scalar x, y;
	}; // !Vector2
	
} // !TGen

#endif // !_TGEN_VECTOR2_H
