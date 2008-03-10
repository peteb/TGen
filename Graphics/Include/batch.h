/*
 *  batch.h
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_BATCH_H
#define _TGEN_BATCH_H

#include "types.h"
#include "renderer.h"
#include "vertexbuffer.h"
#include "error.h"
#include "renderable.h"
#include <iostream>

namespace TGen {
	class VertexStream {
	public:
		VertexStream() {}
		virtual ~VertexStream() {}
		
		virtual void writeVertex(void const * vertex) abstract;
	};
	
	template<typename format, int vertsPerPrimitive>
	class Batch : public TGen::VertexStream, public TGen::Renderable {
	public:	
		Batch(TGen::Renderer & renderer, uint size, TGen::PrimitiveType type, uint usage) 
			: size(size)
			, type(type)
			, vb(NULL)
			, verticesWritten(0)
			, cursor(NULL)
			, mapped(NULL) 
		{
			vb = renderer.createVertexData(format(), sizeof(typename format::Type) * size, usage);
			verticesPerDraw = renderer.getCaps().maxVertexBufferVertices;
		}
		
		~Batch() {
			delete vb;
			vb = NULL;
		}
		
		void preRender(TGen::Renderer & renderer) const {
			if (verticesWritten > 0)
				renderer.setVertexBuffer(vb);
		}
		
		void render(TGen::Renderer & renderer) const {
			if (verticesWritten > 0)
				renderer.drawPrimitive(type, 0, verticesWritten);			
		}
				
		void beginBatch() {
			mapped = vb->lock(TGen::LockDiscard | TGen::LockWrite);
			cursor = mapped;
			verticesWritten = 0; 
			full = false;			
		}
		
		void endBatch() {
			vb->unlock();
			mapped = NULL;			
		}
		
		bool writePrimitive(typename format::Type vertex[vertsPerPrimitive], bool returnOnFull = false) {
			if (!mapped || !cursor)
				throw TGen::RuntimeException("Batch::WritePrimitive", "batch is not ready");
			
			if ((char *)cursor - (char *)mapped >= size) {
				if (returnOnFull) {
					return false; //throw TGen::RuntimeException("Batch::Write", "buffer is full");
				}
				else {
					cursor = mapped;
					full = true;
				}
			}			
			
			memcpy(cursor, vertex, sizeof(typename format::Type) * vertsPerPrimitive);
			cursor = (char *)cursor + sizeof(typename format::Type) * vertsPerPrimitive;
			
			if (!full)
				verticesWritten += vertsPerPrimitive;
			
			return true;
		}
		
		void writeVertex(void const * vertex) {
			if ((char *)cursor - (char *)mapped >= size)	// full
				return;
			
			memcpy(cursor, vertex, sizeof(typename format::Type));
			cursor = reinterpret_cast<char *>(cursor) + sizeof(typename format::Type);
			verticesWritten++;
		}
		
		typedef format Type;
		
	private:
		uint verticesPerDraw;
		uint verticesWritten;
		TGen::VertexData * vb;
		void * cursor, * mapped;
		uint size;
		TGen::PrimitiveType type;
		bool full;
	};
	
} // !TGen

#endif // !_TGEN_BATCH_H
