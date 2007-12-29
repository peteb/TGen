/*
 *  vertexcachentry.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_VERTEXCACHEENTRY_H
#define _TGEN_ENGINE_VERTEXCACHEENTRY_H

#include <tgen_graphics.h>

namespace TGen {
	namespace Engine {
		class VertexCacheEntry2 : public TGen::VertexData {
		private:
			VertexCacheEntry2(TGen::VertexDataSource & creator, uint size, ushort usage, TGen::VertexData * buffer, void * entryAt, uint readAt);
			
		public:	
			~VertexCacheEntry2();
			
			void * lock(ushort flags);
			void unlock();
			void bufferData(const void * data, uint size, void * offset);
			void bindShaderVariable(int id, const TGen::ShaderVariable & var);
			TGen::VertexStructure & getVertexStructure();
			TGen::VertexData * operator * ();
			
			friend class PoolCollection2;
			
		private:
			TGen::VertexData * buffer;		
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_VERTEXCACHEENTRY_H
