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
	
	class ShaderVariable {
	public:	
		ShaderVariable() {}
		virtual ~ShaderVariable() {}
		
		virtual ShaderVariable & operator = (const TGen::Color & color) abstract;
		
	};

} // !TGen

#endif // !_TGEN_SHADERVARIABLE_H
