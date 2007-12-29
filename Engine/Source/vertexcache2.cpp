/*
 *  vertexcache2.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vertexcache2.h"
#include "log.h"
#include "poolcollection2.h"
#include "vertexcacheentry.h"
#include "basicpoolcollection.h"
#include <tgen_graphics.h>

TGen::Engine::VertexCache2::VertexCache2(TGen::VertexDataSource & dataSource, TGen::Engine::StandardLogs & logs)
	: dataSource(dataSource)
	, logs(logs)
{
}

TGen::Engine::VertexCache2::~VertexCache2() {
	for (PoolCollectionMap::iterator iter = poolCollections.begin(); iter != poolCollections.end(); ++iter)
		delete iter->second;
}

TGen::VertexData * TGen::Engine::VertexCache2::createVertexData(const TGen::VertexStructure & vertstruct, uint vertices, ushort usage) {
	return getPoolCollection(vertstruct, usage)->createCacheEntry(vertices);
}

void TGen::Engine::VertexCache2::removeVertexData(TGen::VertexData * data) {

}

std::string TGen::Engine::VertexCache2::getPoolId(const TGen::VertexStructure & vertstruct, ushort usage) const {
	return vertstruct.getStringRepresentation() + "|" + char('A' + (usage & 0xFF)) + char('A' + ((usage >> 8) & 0xFF));
}

TGen::Engine::PoolCollection2 * TGen::Engine::VertexCache2::getPoolCollection(const TGen::VertexStructure & vertstruct, ushort usage) {
	std::string poolId = getPoolId(vertstruct, usage);
	
	PoolCollectionMap::iterator iter = poolCollections.find(poolId);
	if (iter != poolCollections.end())
		return iter->second;
		
	TGen::Engine::PoolCollection2 * newPoolCollection = createPoolCollection(vertstruct, usage);
	poolCollections.insert(PoolCollectionMap::value_type(poolId, newPoolCollection));
	
	return newPoolCollection;
}

TGen::Engine::PoolCollection2 * TGen::Engine::VertexCache2::createPoolCollection(const TGen::VertexStructure & vertstruct, ushort usage) {
	// check rules
	
	return new TGen::Engine::BasicPoolCollection(*this, dataSource, vertstruct, usage, 1000000);
}
