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

namespace TGen {
	class Vector2;
	
	class Matrix3x3 {
	public:	
		Matrix3x3();
		Matrix3x3(const TGen::Vector2 & x, const TGen::Vector2 & y);
		Matrix3x3(const TGen::Matrix3x3 & matrix);
		Matrix3x3(float e11, float e12, float e13, float e21, float e22, float e23, float e31, float e32, float e33);
		
		TGen::Matrix3x3 & operator = (const TGen::Matrix3x3 & matrix);
		TGen::Vector2 operator * (const TGen::Vector2 & vector) const;
		TGen::Matrix3x3 operator * (const TGen::Matrix3x3 & matrix) const;
		operator std::string() const;
		
		void setOrigin(const TGen::Vector2 & origin);
		void setScale(const TGen::Vector2 & scale);
		void get4x4(float * elements) const;
		
		static TGen::Matrix3x3 Identity;
		static TGen::Matrix3x3 Translation(const TGen::Vector2 & vector);
		static TGen::Matrix3x3 Scaling(const TGen::Vector2 & scale);
		
		float elements[3][3];
		
	private:
		void Clear();
	};	
} // !TGen

#endif // !_TGEN_MATRIX3X3_H
