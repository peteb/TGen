/*
 *  shadervariable.h
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_SHADERVARIABLE_H
#define _TGEN_SHADERVARIABLE_H

#include "types.h"

namespace TGen {
	class Color;
	class Vector4;
	class Vector3;
	class Vector2;
	class Matrix4x4;
	
	class ShaderVariable {
	public:	
		ShaderVariable() {}
		virtual ~ShaderVariable() {}

		virtual ShaderVariable & operator = (int value) abstract;
		virtual ShaderVariable & setInt(int value) abstract;

		virtual ShaderVariable & operator = (float value) abstract;
		virtual ShaderVariable & operator = (const TGen::Color & color) abstract;
		virtual ShaderVariable & operator = (const TGen::Vector4 & vector) abstract;		
		virtual ShaderVariable & operator = (const TGen::Vector3 & vector) abstract;
		virtual ShaderVariable & operator = (const TGen::Vector2 & vector) abstract;
		virtual ShaderVariable & operator = (const TGen::Matrix4x4 & matrix) abstract;
		
		
	};

} // !TGen

#endif // !_TGEN_SHADERVARIABLE_H
