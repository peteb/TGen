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
#include <iostream>

namespace TGen {
	template<typename format, int vertsPerPrimitive>
	class Batch {
	public:	
		Batch(TGen::Renderer * renderer, uint size, TGen::PrimitiveType type, uint usage) 
			: size(size)
			, type(type)
			, vb(NULL)
			, verticesWritten(0)
			, cursor(NULL)
			, mapped(NULL) 
		{
			vb = renderer->CreateVertexBuffer(format(), sizeof(typename format::Type) * size, usage);
			verticesPerDraw = renderer->getCaps().maxVertexBufferVertices;
		}
		
		~Batch() {
			delete vb;
			vb = NULL;
		}
		
		void Render(TGen::Renderer * renderer) {
			if (verticesWritten > 0) {
				renderer->setVertexBuffer(vb);
				
				for (int i = 0; i < verticesWritten; i += verticesPerDraw)
					renderer->DrawPrimitive(type, i, std::min(verticesPerDraw, verticesWritten));
				//renderer->DrawPrimitive(type, 0, verticesWritten);
			}
			
		}
		
		void BeginBatch() {
			mapped = vb->Lock(TGen::LockDiscard | TGen::LockWrite);
			cursor = mapped;
			verticesWritten = 0; 
			full = false;			
		}
		
		void EndBatch() {
			vb->Unlock();
			mapped = NULL;			
		}
		
		bool WritePrimitive(typename format::Type vertex[vertsPerPrimitive], bool returnOnFull = false) {
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
		
		typedef format Type;
		
	private:
		uint verticesPerDraw;
		uint verticesWritten;
		TGen::VertexBuffer * vb;
		void * cursor, * mapped;
		uint size;
		TGen::PrimitiveType type;
		bool full;
	};
	
} // !TGen

#endif // !_TGEN_BATCH_H
