/*
 *  matrix4x4.h
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_MATRIX4X4_H
#define _TGEN_MATRIX4X4_H

#include <string>
#include "types.h"

namespace TGen {
	class Matrix3x3;
	class Vector3;
	
	class Matrix4x4 {
	public:
		Matrix4x4();
		Matrix4x4(const TGen::Vector3 & x, const TGen::Vector3 & y, const TGen::Vector3 & z);
		Matrix4x4(const TGen::Matrix4x4 & matrix);
		Matrix4x4(const TGen::Matrix3x3 & matrix);		
		Matrix4x4(scalar e11, scalar e12, scalar e13, scalar e14, scalar e21, scalar e22, scalar e23, scalar e24, scalar e31, scalar e32, scalar e33, scalar e34, scalar e41, scalar e42, scalar e43, scalar e44);
		
		TGen::Matrix4x4 & operator = (const TGen::Matrix4x4 & matrix);
		TGen::Vector3 operator * (const TGen::Vector3 & vector) const;
		TGen::Matrix4x4 operator * (const TGen::Matrix4x4 & matrix) const;
		operator std::string() const;
		
		void setOrigin(const TGen::Vector3 & origin);
		void setScale(const TGen::Vector3 & scale);
		void get4x4(scalar * elements) const;
		
		static TGen::Matrix4x4 Identity;
		static TGen::Matrix4x4 Translation(const TGen::Vector3 & vector);
		static TGen::Matrix4x4 Scaling(const TGen::Vector3 & scale);
		
		scalar elements[4][4];
		
	private:
		void Clear();
		
	}; // !Matrix4x4
	
} // !TGen

#endif // !_TGEN_MATRIX4X4_H
