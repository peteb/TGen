/*
 *  frustum.h
 *  TGen Math
 *
 *  Created by Peter Backman on 7/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_FRUSTUM_H
#define _TGEN_FRUSTUM_H

#include "plane3.h"

namespace TGen {
	class Matrix4x4;
	class Sphere;
	class AABB;
	class Vector3;
	
	enum Planes {
		FarPlane = 0,
		NearPlane,
		RightPlane,
		LeftPlane,
		BottomPlane,
		TopPlane,
	};
	
	class Frustum {
	public:
		Frustum() {}
		Frustum(const TGen::Matrix4x4 & projection, const TGen::Matrix4x4 & worldview);
		
		void calculate(const TGen::Matrix4x4 & projection, const TGen::Matrix4x4 & worldview);
		int intersects(const TGen::Sphere & sphere) const;
		int intersects(const TGen::AABB & aabb) const;
		int intersects(const TGen::Vector3 & point) const;
		
		TGen::Plane3 nearPlane, farPlane, rightPlane, leftPlane, topPlane, bottomPlane;
	};
} // !TGen

#endif // !_TGEN_FRUSTUM_H
