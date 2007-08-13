/*
 *  vector4.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 7/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vector4.h"
#include "vector3.h"
#include "vector2.h"

TGen::Vector4 TGen::Vector4::Identity(1.0, 1.0, 1.0, 1.0);

TGen::Vector4::Vector4(scalar x, scalar y, scalar z, scalar w)
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{
		
}

TGen::Vector4::Vector4(const Vector3 & vector)
	: x(vector.x)
	, y(vector.y)
	, z(vector.z)
	, w(0.0)
{
		
}

TGen::Vector4::Vector4(const Vector2 & vector)
	: x(vector.x)
	, y(vector.y)
	, z(0.0)
	, w(0.0)
{
		
}
