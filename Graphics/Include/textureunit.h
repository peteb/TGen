/*
 *  textureunit.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 8/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_TEXTUREUNIT_H
#define _TGEN_TEXTUREUNIT_H

#include "renderer_types.h"
#include <tgen_math.h>

namespace TGen {
	class Texture;
	
	class TextureUnit {
	public:
		TextureUnit(int unit, TGen::Texture * texture);
		TextureUnit(int unit, int textureType);
		
		int unit;
		TGen::Texture * texture;
		TGen::TextureCoordGen genU, genV;
		TGen::Matrix4x4 transform;
		bool transformed;
		int textureType;
	};	
	
} // !TGen

#endif // !_TGEN_TEXTUREUNIT_H
