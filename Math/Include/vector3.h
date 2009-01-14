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
#include "vector2.h"
#include "vector4.h"
#include <cmath>

namespace TGen {
	class Vector3 {
	public:	
		Vector3(const TGen::Vector4 & vector) : x(vector.x), y(vector.y), z(vector.z) {}
		Vector3(const TGen::Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z) {}
		Vector3(const TGen::Vector2 & vector) : x(vector.x), y(vector.y), z(0.0f) {}
		Vector3(scalar x, scalar y, scalar z) : x(x), y(y), z(z) {}
		Vector3(scalar value) : x(value), y(value), z(value) {}
		Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		
		bool operator == (const TGen::Vector3 & vector) const;
		Vector3 & operator += (const TGen::Vector3 & vector);
		Vector3 & operator -= (const TGen::Vector3 & vector);
		Vector3 & operator = (const TGen::Vector3 & vector);
		Vector3 & operator /= (scalar scal);
		Vector3 & operator *= (scalar scalar);
		
		Vector3 operator + (const TGen::Vector3 & vector) const;
		Vector3 operator - (const TGen::Vector3 & vector) const;
		Vector3 operator * (scalar value) const;
		Vector3 operator * (const TGen::Vector3 & vector) const;
		Vector3 operator / (scalar value) const;
		Vector3 operator - () const;
		
		bool operator < (const TGen::Vector3 & vector) const;
		bool operator > (const TGen::Vector3 & vector) const;
		
		operator std::string() const;
		
		scalar getMagnitude() const {return fabs(sqrt(x * x + y * y + z * z)); }
		
		Vector3 getNormalized() const {return TGen::Vector3(*this).normalize(); }
		
		Vector3 & normalize() {
			scalar magnitude = getMagnitude();
			if (magnitude == 0.0f)
				return *this;
			
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
			
			return *this;
		}
		
		static scalar DotProduct(const TGen::Vector3 & v1, const TGen::Vector3 & v2);
		static TGen::Vector3 CrossProduct(const TGen::Vector3 & v1, const TGen::Vector3 & v2);
		static TGen::Vector3 Parse(const std::string & text);
		static TGen::Vector3 Min(const TGen::Vector3 & v1, const TGen::Vector3 & v2);
		static TGen::Vector3 Max(const TGen::Vector3 & v1, const TGen::Vector3 & v2);
		
		static TGen::Vector3 Identity;
		static TGen::Vector3 Zero;
		
		scalar x, y, z;
	};
	
} // !TGen

#endif // !_TGEN_VECTOR3_H
