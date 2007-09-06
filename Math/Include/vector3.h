/*
 *  vector3.h
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_VECTOR3_H
#define _TGEN_VECTOR3_H

#include <string>
#include "types.h"

namespace TGen {
	class Vector2;
	
	class Vector3 {
	public:	
		Vector3(const TGen::Vector3 & vector);
		Vector3(const TGen::Vector2 & vector);
		Vector3(scalar x, scalar y, scalar z);
		Vector3(scalar scalar = 0.0f);

		bool operator == (const TGen::Vector3 & vector) const;
		Vector3 & operator += (const TGen::Vector3 & vector);
		Vector3 & operator -= (const TGen::Vector3 & vector);
		Vector3 & operator = (const TGen::Vector3 & vector);
		Vector3 & operator /= (scalar scal);
		Vector3 & operator *= (scalar scalar);
		
		Vector3 operator + (const TGen::Vector3 & vector) const;
		Vector3 operator - (const TGen::Vector3 & vector) const;
		Vector3 operator * (scalar scalar) const;
		Vector3 operator * (const TGen::Vector3 & vector) const;
		Vector3 operator / (scalar scalar) const;
		Vector3 operator - () const;
		
		bool operator < (const TGen::Vector3 & vector) const;
		bool operator > (const TGen::Vector3 & vector) const;
		
		operator std::string() const;
		
		scalar getMagnitude() const;
		Vector3 getNormalized() const;
		Vector3 & normalize();
		
		static scalar DotProduct(const TGen::Vector3 & v1, const TGen::Vector3 & v2);
		static TGen::Vector3 CrossProduct(const TGen::Vector3 & v1, const TGen::Vector3 & v2);
		
		static TGen::Vector3 Identity;
		
		scalar x, y, z;
	};
	
} // !TGen

#endif // !_TGEN_VECTOR3_H
