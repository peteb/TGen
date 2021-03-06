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
		Vector2(const TGen::Vector2 & vector) throw();
		Vector2(const TGen::Vector3 & vector) throw();
		Vector2(scalar x, scalar y) throw();
		Vector2(scalar scalar = 0.0f) throw();
		
		Vector2 & operator += (const TGen::Vector2 & vector) throw();
		Vector2 & operator -= (const TGen::Vector2 & vector) throw();
		Vector2 & operator *= (const TGen::Vector2 & vector) throw();
		Vector2 & operator = (const TGen::Vector2 & vector) throw();
		Vector2 & operator *= (scalar scalar) throw();
		
		Vector2 operator + (const TGen::Vector2 & vector) const throw();
		Vector2 operator - (const TGen::Vector2 & vector) const throw();
		Vector2 operator * (const TGen::Vector2 & vector) const throw();		
		Vector2 operator * (scalar _scalar) const throw();
		Vector2 operator / (scalar _scalar) const throw();
		Vector2 operator - () const throw();
		
		operator std::string() const;
		
		scalar getMagnitude() const throw();
		Vector2 getNormalized() const throw();
		Vector2 & normalize() throw();
		Vector2 getPerpendicular() const throw();
		
		static scalar DotProduct(const TGen::Vector2 & v1, const TGen::Vector2 & v2) throw();
		static TGen::Vector2 CrossProduct(const TGen::Vector2 & v1, const TGen::Vector2 & v2) throw();
		static TGen::Vector2 Identity;
		static TGen::Vector2 Zero;
		static TGen::Vector2 Parse(const std::string & text);


		scalar x, y;
	}; // !Vector2
	
} // !TGen

#endif // !_TGEN_VECTOR2_H
