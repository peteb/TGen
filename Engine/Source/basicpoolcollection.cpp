/*
 *  basicpoolcollection.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "basicpoolcollection.h"

TGen::Engine::BasicPoolCollection::BasicPoolCollection(TGen::Engine::VertexCache2 & creator, TGen::VertexDataSource & dataSource, 
																		 const TGen::VertexStructure & vertstruct, ushort usage, uint freeSlabVertices)
	: TGen::Engine::PoolCollection2(creator, dataSource, vertstruct, usage)
	, freeSlabVertices(freeSlabVertices)
{
}

TGen::Engine::BasicPoolCollection::~BasicPoolCollection() {
	
}

TGen::Engine::VertexCacheEntry2 * TGen::Engine::BasicPoolCollection::createCacheEntry(uint vertices) {
	
	return NULL;
}

void TGen::Engine::BasicPoolCollection::freeCacheEntry(TGen::Engine::VertexCacheEntry2 * entry) {
	
}

