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
		
		TGen::Texture * getTexture() const;
		void setCoordGen(TGen::TextureCoordGen genU, TGen::TextureCoordGen genV);
		void getCoordGen(TGen::TextureCoordGen & genU, TGen::TextureCoordGen & genV) const;
		void setFilter(TGen::TextureFilter minFilter, TGen::TextureFilter magFilter);
		TGen::TextureFilter getMinFilter() const;
		TGen::TextureFilter getMagFilter() const;
		
		int getTextureType() const;
		int getUnit() const;
		
		TGen::Matrix4x4 transform;
		bool transformed;

	private:
		int unit;
		TGen::Texture * texture;
		TGen::TextureCoordGen genU, genV;
		TGen::TextureFilter minFilter, magFilter;
		int textureType;
	};	
	
} // !TGen

#endif // !_TGEN_TEXTUREUNIT_H
