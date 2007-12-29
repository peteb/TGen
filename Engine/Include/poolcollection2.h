/*
 *  poolcollection2.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_POOLCOLLECTION2_H
#define _TGEN_ENGINE_POOLCOLLECTION2_H

#include <tgen_core.h>
#include <tgen_graphics.h>

namespace TGen {
	namespace Engine {
		class VertexCacheEntry2;
		class VertexCache2;
		
		class PoolCollection2 {
		public:	
			PoolCollection2(TGen::Engine::VertexCache2 & creator, TGen::VertexDataSource & dataSource, const TGen::VertexStructure & vertstruct, ushort usage);
			virtual ~PoolCollection2() {}
			
			virtual TGen::Engine::VertexCacheEntry2 * createCacheEntry(uint vertices) abstract;
			virtual void freeCacheEntry(TGen::Engine::VertexCacheEntry2 * entry) abstract;
			
		protected:
			TGen::Engine::VertexCache2 & creator;
			TGen::VertexDataSource & dataSource;
			TGen::VertexStructure vertexStructure;
			ushort usage;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_POOLCOLLECTION2_H
