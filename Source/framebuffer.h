/*
 *  framebuffer.h
 *  tgen
 *
 *  Created by Peter Backman on 6/26/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_FRAMEBUFFER_H
#define _TGEN_FRAMEBUFFER_H

#include "types.h"

namespace TGen {
	enum FrameBufferAttachment {
		ColorAttachment = 1,
		DepthAttachment,
		StencilAttachment,		
	};
	
	class Texture;
	
	class FrameBuffer {
	public:
		FrameBuffer() {}
		virtual ~FrameBuffer() {}
		
		virtual	void Attach(const TGen::Texture * texture, TGen::FrameBufferAttachment attachpoint) abstract;
	};
} // !TGen

#endif // !_TGEN_FRAMEBUFFER_H
