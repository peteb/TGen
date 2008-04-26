/*
 *  matrix3x3.h
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_MATRIX3X3_H
#define _TGEN_MATRIX3X3_H

#include <string>
#include "types.h"

namespace TGen {
	class Vector2;
	class Vector3;
	class Matrix4x4;
	class Angle;
	
	class Matrix3x3 {
	public:	
		Matrix3x3();
		Matrix3x3(const TGen::Vector2 & x, const TGen::Vector2 & y);
		Matrix3x3(const TGen::Vector3 & x, const TGen::Vector3 & y, const TGen::Vector3 & z);
		Matrix3x3(const TGen::Matrix3x3 & matrix);
		Matrix3x3(const TGen::Matrix4x4 & matrix);
		Matrix3x3(scalar e11, scalar e12, scalar e13, scalar e21, scalar e22, scalar e23, scalar e31, scalar e32, scalar e33);
		
		TGen::Matrix3x3 & operator = (const TGen::Matrix3x3 & matrix);
		TGen::Vector2 operator * (const TGen::Vector2 & vector) const;
		TGen::Matrix3x3 operator * (const TGen::Matrix3x3 & matrix) const;
		TGen::Matrix3x3 & operator *= (const TGen::Matrix3x3 & matrix);
		
		scalar & operator () (int x, int y);
		const scalar & operator () (int x, int y) const;
		
		operator std::string() const;
		Matrix3x3 & orthogonalize();
		
		void setOrigin(const TGen::Vector2 & origin);
		void setScale(const TGen::Vector2 & scale);
		void get4x4(scalar * elements) const;

		TGen::Vector3 getX() const;
		TGen::Vector3 getY() const;	
		TGen::Vector3 getZ() const;
		void setX(const TGen::Vector3 & x);
		void setY(const TGen::Vector3 & x);
		void setZ(const TGen::Vector3 & x);
		
		static TGen::Matrix3x3 Identity;
		static TGen::Matrix3x3 Translation(const TGen::Vector2 & vector);
		static TGen::Matrix3x3 Scaling(const TGen::Vector2 & scale);
		static TGen::Matrix3x3 LookInDirection(const TGen::Vector3 & direction, const TGen::Vector3 & up);
		static TGen::Matrix3x3 RotationZ(const TGen::Angle & angle);
		static TGen::Matrix3x3 RotationX(const TGen::Angle & angle);
		static TGen::Matrix3x3 RotationY(const TGen::Angle & angle);
		
		scalar elements[3][3];
		
	private:
		void Clear();
	};	
} // !TGen

#endif // !_TGEN_MATRIX3X3_H
