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
	class Vector4;
	class Rectangle;
	class Angle;
	class Degree;
	
	class Matrix4x4 {
	public:
		Matrix4x4() {Clear(); }
		Matrix4x4(const TGen::Vector3 & x, const TGen::Vector3 & y, const TGen::Vector3 & z);
		Matrix4x4(const TGen::Matrix4x4 & other) {memcpy(&elements, &other.elements, 4 * 4 * sizeof(scalar));	}
		//Matrix4x4(const TGen::Matrix4x4 & matrix);
		Matrix4x4(const TGen::Matrix3x3 & matrix);		
		Matrix4x4(scalar e11, scalar e12, scalar e13, scalar e14, scalar e21, scalar e22, scalar e23, scalar e24, scalar e31, scalar e32, scalar e33, scalar e34, scalar e41, scalar e42, scalar e43, scalar e44);
		
		TGen::Matrix4x4 & operator = (const TGen::Matrix4x4 & matrix);
		TGen::Vector3 operator * (const TGen::Vector3 & vector) const;
		TGen::Vector4 operator * (const TGen::Vector4 & vector) const;

		TGen::Matrix4x4 operator * (const TGen::Matrix4x4 & matrix) const;
		TGen::Matrix4x4 operator * (const TGen::Matrix3x3 & matrix) const;
		
		TGen::Matrix4x4 & operator *= (const TGen::Matrix4x4 & matrix);

		operator std::string() const;
		scalar & operator () (int x, int y);
		const scalar & operator () (int x, int y) const;

		Matrix4x4 & transpose();
		Matrix4x4 & invert();
		Matrix4x4 getInverse() const;
		Matrix4x4 & orthonormalize();
		
		void setDiagonal(scalar value) {elements[0][0] = elements[1][1] = elements[2][2] = elements[3][3] = value; }
		void setOrigin(const TGen::Vector3 & origin);
		void setScale(const TGen::Vector3 & scale);
		void get4x4(scalar * elements) const;
		TGen::Vector3 getOrigin() const;
		TGen::Vector3 getX() const;
		TGen::Vector3 getY() const;
		TGen::Vector3 getZ() const;
		void setX(const TGen::Vector3 & xaxis);
		void setY(const TGen::Vector3 & yaxis);
		void setZ(const TGen::Vector3 & zaxis);
		
		static TGen::Matrix4x4 Identity;
		static TGen::Matrix4x4 Translation(const TGen::Vector3 & vector);
		static TGen::Matrix4x4 Scaling(const TGen::Vector3 & scale);
		static TGen::Matrix4x4 OrthogonalProjection(scalar left, scalar top, scalar right, scalar bottom, scalar near = -1.0, scalar far = 1.0);
		static TGen::Matrix4x4 OrthogonalProjection(const TGen::Rectangle & area, scalar near = -1.0, scalar far = 1.0);
		static TGen::Matrix4x4 PerspectiveProjection(const TGen::Angle & fieldOfViewY, scalar fieldOfViewAspect, scalar near, scalar far);
		static TGen::Matrix4x4 Rotation(const TGen::Vector3 & axis, const TGen::Angle & angle);
		static TGen::Matrix4x4 RotationX(const TGen::Angle & angle);
		static TGen::Matrix4x4 RotationY(const TGen::Angle & angle);
		static TGen::Matrix4x4 RotationZ(const TGen::Angle & angle);

		static TGen::Matrix4x4 LookAt(const TGen::Vector3 & position, const TGen::Vector3 & eye, const TGen::Vector3 & up);
		static TGen::Matrix4x4 LookInDirection(const TGen::Vector3 & direction, const TGen::Vector3 & up);
		
		scalar elements[4][4];
		
	private:
		void Clear();
		
	}; // !Matrix4x4
	
} // !TGen

#endif // !_TGEN_MATRIX4X4_H
