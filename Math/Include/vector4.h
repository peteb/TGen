/*
 *  vector4.h
 *  TGen Math
 *
 *  Created by Peter Backman on 7/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_VECTOR4_H
#define _TGEN_VECTOR4_H

#include <string>
#include "types.h"

namespace TGen {
	class Vector3;
	class Vector2;
	
	class Vector4 {
	public:
		Vector4();
		Vector4(scalar x, scalar y, scalar z, scalar w);
		Vector4(const Vector3 & vector, scalar w = 1.0f);
		Vector4(const Vector2 & vector);
		
		TGen::Vector4 & operator /= (scalar div);
		
		operator std::string() const;

		static Vector4 Identity;
		
		scalar x, y, z, w;		
	};
	
	
} // !TGen

#endif // !_TGEN_VECTOR4_H
