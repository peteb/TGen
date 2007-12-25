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
		class StandardLogs;
		
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
			VertexCache(TGen::VertexDataSource & dataSource, TGen::Engine::StandardLogs & logs);
			~VertexCache();
			
			VertexData * createVertexData(const VertexStructure & vertstruct, uint size, ushort usage);
			void lockDynamicPools();
			void unlockDynamicPools();
			
		private:
			void removeVertexData(VertexData * data);

			TGen::VertexDataSource & dataSource;
			TGen::Engine::StandardLogs & logs;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_VERTEXCACHE_H
