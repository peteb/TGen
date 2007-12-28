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
		class VertexCache;
		
		class VertexCacheEntry : public TGen::VertexData {
		private:
			VertexCacheEntry(TGen::VertexDataSource & creator, uint size, ushort usage, TGen::VertexData * buffer, void * entryAt, uint readAt);
			
		public:	
			~VertexCacheEntry();
			
			void * lock(ushort flags);
			void unlock();
			void bufferData(const void * data, uint size, void * offset);
			void bindShaderVariable(int id, const TGen::ShaderVariable & var);
			TGen::VertexStructure & getVertexStructure();
			TGen::VertexData * operator * ();
			
			friend class PoolCollection;
			
		private:
			TGen::VertexData * buffer;
			void * entryAt;
		};
		
		class PoolCollection {
		public:
			PoolCollection(const TGen::VertexStructure & vertstruct, ushort usage, TGen::Engine::VertexCache & cache);
			
			TGen::Engine::VertexCacheEntry * alloc(uint size);
			void free(TGen::Engine::VertexCacheEntry * entry);
			
		private:
			TGen::Engine::VertexCache & cache;
			TGen::VertexStructure vertstruct;
			ushort usage;
			
			// test:
			TGen::VertexData * buffer;
			void * dataAt;
			uint sizeLeft;
		};
		
		/*
			Varje PoolCollection har 0 eller flera dataslabs allokerade, sen har varje Pool i samlingen en pekare till vilken dataslab den senast allokerade i.
			Storlekar anges i vertices
		 
		 
		 
		 
		 */
		
		class VertexCache : public TGen::VertexDataSource {
		public:
			VertexCache(TGen::VertexDataSource & dataSource, TGen::Engine::StandardLogs & logs);
			~VertexCache();
			
			VertexData * createVertexData(const VertexStructure & vertstruct, uint size, ushort usage);
			void lockDynamicPools();
			void unlockDynamicPools();
			TGen::VertexDataSource & getDataSource() const;
			
		private:
			void removeVertexData(VertexData * data);
			TGen::Engine::PoolCollection * getPoolCollection(const TGen::VertexStructure & vertstruct, ushort usage);
			
			typedef std::map<std::string, PoolCollection *> PoolCollectionMap;
			
			PoolCollectionMap poolCollections;
			TGen::VertexDataSource & dataSource;
			TGen::Engine::StandardLogs & logs;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_VERTEXCACHE_H
