/*
 *  basicpoolcollection.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_BASICPOOLCOLLECTION_H
#define _TGEN_ENGINE_BASICPOOLCOLLECTION_H

#include "poolcollection2.h"

namespace TGen {
	namespace Engine {
		class VertexStructure;
		
		class BasicPoolCollection : public TGen::Engine::PoolCollection2 {
		public:	
			BasicPoolCollection(TGen::Engine::VertexCache2 & creator, TGen::VertexDataSource & dataSource, const TGen::VertexStructure & vertstruct, ushort usage, uint freeSlabVertices);
			~BasicPoolCollection();
			
			TGen::Engine::VertexCacheEntry2 * createCacheEntry(uint vertices);
			void freeCacheEntry(TGen::Engine::VertexCacheEntry2 * entry);
			
		private:
			uint freeSlabVertices;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_BASICPOOLCOLLECTION_H
