/*
 *  rendercontext.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERCONTEXT_H
#define _TGEN_RENDERCONTEXT_H

#include "color.h"
#include <map>

namespace TGen {
	class Shader;
	class Texture;
	
	class RenderContext {
	public:
		RenderContext();
		
		void setTextureUnit(int unit, TGen::Texture * texture);
		
		bool depthWrite;
		TGen::Color frontColor;
		TGen::Shader * shader;
		
		typedef std::map<int, TGen::Texture *> TextureMap;
		TextureMap textureUnits;
	};
	
} // !TGen

#endif // !_TGEN_RENDERCONTEXT_H
