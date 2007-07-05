/*
 *  renderable.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERABLE_H
#define _TGEN_RENDERABLE_H

#include "tgen_core.h"

namespace TGen {
	class Renderer;
	
	class Renderable {
	public:
		Renderable() {}
		virtual ~Renderable() {}
		
		virtual void PrepareRender(TGen::Renderer & renderer) abstract;
		virtual void Render(TGen::Renderer & renderer) abstract;
		
	};
} // !TGen

#endif // !_TGEN_RENDERABLE_H