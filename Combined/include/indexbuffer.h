/*
 *  indexbuffer.h
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_INDEXBUFFER_H
#define _TGEN_INDEXBUFFER_H

#include "types.h"

namespace TGen {
	class Renderer;
	class VertexStructure;
	
	class IndexBuffer {
	protected:
		IndexBuffer(TGen::Renderer & creator, uint size, ushort usage);
		
	public:
		virtual ~IndexBuffer();
		
		virtual void * Lock(ushort flags) abstract;
		virtual void Unlock() abstract;
		virtual void BufferData(void * data, uint size, void * offset) abstract;
		virtual bool isLocked() abstract;
		virtual TGen::VertexStructure & getVertexStructure() abstract;
		
		uint getSize() const;
		
	protected:
		uint size, usage;
		TGen::Renderer & creator;
	};	
} // !TGen


#endif // !_TGEN_INDEXBUFFER_H
