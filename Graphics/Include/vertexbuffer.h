/*
 *  vertexbuffer.h
 *  tgen
 *
 *  Created by Peter Backman on 6/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_VERTEXBUFFER_H
#define _TGEN_VERTEXBUFFER_H

#include "types.h"

namespace TGen {
	class Renderer;
	class VertexStructure;
	
	class VertexBuffer {
	protected:
		VertexBuffer(TGen::Renderer & creator, uint size, ushort usage);
		
	public:
		virtual ~VertexBuffer();
		
		virtual void * lock(ushort flags) abstract;
		virtual void unlock() abstract;
		virtual void bufferData(const void * data, uint size, void * offset) abstract;
		virtual bool isLocked() abstract;
		virtual TGen::VertexStructure & getVertexStructure() abstract;
		
		uint getSize() const;
		
	protected:
		uint size, usage;
		TGen::Renderer & creator;
	};
	
} // !TGen

#endif // !_TGEN_VERTEXBUFFER_H
