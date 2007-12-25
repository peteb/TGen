/*
 *  vertexcache.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_VERTEXCACHE_H
#define _TGEN_ENGINE_VERTEXCACHE_H

#include <tgen_graphics.h>

namespace TGen {
	namespace Engine {
		class VertexCacheEntry : public TGen::VertexData {
		protected:
			VertexCacheEntry(TGen::VertexDataSource & creator, uint size, ushort usage);
			
		public:	
			void * lock(ushort flags);
			void unlock();
			void bufferData(const void * data, uint size, void * offset);
			void bindShaderVariable(int id, const TGen::ShaderVariable & var);
			TGen::VertexStructure & getVertexStructure();
			
		};
		
		class VertexCache : public TGen::VertexDataSource {
		public:
			VertexData * createVertexData(const VertexStructure & vertstruct, uint size, ushort usage);

		private:
			void removeVertexData(VertexData * data);

		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_VERTEXCACHE_H
