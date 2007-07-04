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

namespace TGen {
	class RenderContext {
	public:
		RenderContext();
		
		bool depthWrite;
		TGen::Color frontColor;
	};
	
} // !TGen

#endif // !_TGEN_RENDERCONTEXT_H
