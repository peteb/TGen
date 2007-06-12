/*
 *  renderer.h
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_H
#define _TGEN_RENDERER_H

#include "color.h"
#include "types.h"

namespace TGen {
	enum Buffers {
		ColorBuffer		= 0x0001,
		DepthBuffer		= 0x0002,
		StencilBuffer	= 0x0004,
	};
	
	class Renderer {
	protected:
		Renderer();
		
	public:
		virtual ~Renderer();
		
		virtual void setClearColor(const TGen::Color & color);
		virtual TGen::Color getClearColor() const;
		virtual void Clear(ushort buffers) abstract;
		
	private:
		TGen::Color clearColor;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_H
