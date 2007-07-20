/*
 *  texture.h
 *  tgen
 *
 *  Created by Peter Backman on 6/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_TEXTURE_H
#define _TGEN_TEXTURE_H

#include "rectangle.h"

namespace TGen {
	class Renderer;
	
	class Texture {
	protected:
		Texture(TGen::Renderer & creator, const TGen::Rectangle & size);
		
	public:
		virtual ~Texture() {}
		
		const TGen::Rectangle size;
		
	protected:
		TGen::Renderer & creator;
	};
	
	
} // !TGen

#endif // !_TGEN_TEXTURE_H
