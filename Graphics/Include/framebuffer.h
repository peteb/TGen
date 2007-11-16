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
	enum FramebufferAttachment {
		FramebufferAttachmentColor = 1,
		FramebufferAttachmentDepth,
		FramebufferAttachmentStencil,		
	};
	
	class Texture;
	
	class FrameBuffer {
	public:
		FrameBuffer() {}
		virtual ~FrameBuffer() {}
		
		virtual void attachColor(TGen::Texture * texture) abstract;
		virtual void attachDepth(TGen::Texture * texture) abstract;
		virtual void attachStencil(TGen::Texture * texture) abstract;
		virtual void attach(TGen::Texture * texture, TGen::FramebufferAttachment attachpoint) abstract;
	};
} // !TGen

#endif // !_TGEN_FRAMEBUFFER_H
