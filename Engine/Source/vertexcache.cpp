/*
 *  vertexcache.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vertexcache.h"
#include "log.h"
#include "fillquad.h"

TGen::Engine::VertexCacheEntry::VertexCacheEntry(TGen::VertexDataSource & creator, uint size, ushort usage, TGen::VertexData * buffer, void * entryAt, uint readAt)
	: TGen::VertexData(creator, size, usage, entryAt, readAt)
	, buffer(buffer)
	, entryAt(entryAt)
{
}

TGen::Engine::VertexCacheEntry::~VertexCacheEntry() {
	std::cout << "remove this cache entry" << std::endl;
}

void * TGen::Engine::VertexCacheEntry::lock(ushort flags) {
	std::cout << "VERTEX CACHE ENTRY LOCK!!!" << std::endl;

	void * bufferLockedAt = buffer->lock(flags);
	
	return reinterpret_cast<char *>(bufferLockedAt) + reinterpret_cast<uint>(entryAt);
}

void TGen::Engine::VertexCacheEntry::unlock() {
	std::cout << "VERTEX CACHE ENTRY UNLOCK!!!" << std::endl;

	buffer->unlock();
}

void TGen::Engine::VertexCacheEntry::bufferData(const void * data, uint size, void * offset) {
	//std::cout << "VERTEX CACHE ENTRY BUFFERDATA!!!" << std::endl;
	buffer->bufferData(data, size, reinterpret_cast<char *>(entryAt) + reinterpret_cast<uint>(offset));
}

void TGen::Engine::VertexCacheEntry::bindShaderVariable(int id, const TGen::ShaderVariable & var) {
	buffer->bindShaderVariable(id, var);
}

TGen::VertexStructure & TGen::Engine::VertexCacheEntry::getVertexStructure() {
	return buffer->getVertexStructure();
}

TGen::VertexData * TGen::Engine::VertexCacheEntry::operator * () {
	return buffer;
}


TGen::Engine::VertexCache::VertexCache(TGen::VertexDataSource & dataSource, TGen::Engine::StandardLogs & logs) 
	: dataSource(dataSource)
	, logs(logs)
{
	getPoolCollection(TGen::Engine::FillQuad::VertexDecl(), TGen::UsageStatic);
	
	logs.info["vcache+"] << "initialized" << TGen::endl;
}

TGen::Engine::VertexCache::~VertexCache() {
	logs.info["vcache-"] << "shutting down..." << TGen::endl;
}

TGen::VertexData * TGen::Engine::VertexCache::createVertexData(const VertexStructure & vertstruct, uint size, ushort usage) {
	std::string poolId = vertstruct.getStringRepresentation() + "|" + char('A' + (usage & 0xFF)) + char('A' + ((usage >> 8) & 0xFF));

	logs.info["vcache"] << "someone's asking for " << size << " bytes, poolId: " << poolId << TGen::endl;
	
	
	TGen::Engine::VertexCacheEntry * entry = getPoolCollection(vertstruct, usage)->alloc(size);
	
	//if (size == 64)
		return entry; //dataSource.createVertexData(vertstruct, size, usage);
	//else
		//return dataSource.createVertexData(vertstruct, size, usage);
}

TGen::Engine::PoolCollection * TGen::Engine::VertexCache::getPoolCollection(const TGen::VertexStructure & vertstruct, ushort usage) {
	std::string poolId = vertstruct.getStringRepresentation() + "|" + char('A' + (usage & 0xFF)) + char('A' + ((usage >> 8) & 0xFF));

	PoolCollectionMap::iterator iter = poolCollections.find(poolId);
	
	if (iter != poolCollections.end()) {
		logs.info["vcache"] << "found pool collection" << TGen::endl;
		return iter->second;		
	}
	else {
		logs.info["vcache"] << "no pool collection created, creating one..." << TGen::endl;
		TGen::Engine::PoolCollection * poolCollection = new TGen::Engine::PoolCollection(vertstruct, usage, *this);
		poolCollections.insert(PoolCollectionMap::value_type(poolId, poolCollection));

		return poolCollection;
	}
}


void TGen::Engine::VertexCache::removeVertexData(TGen::VertexData * data) {
	/*logs.info["vcache"] << "vertex cache entry remove..." << TGen::endl;
	
	TGen::Engine::VertexCacheEntry * entry = static_cast<TGen::Engine::VertexCacheEntry *>(data);
	
	ushort usage = entry->getUsage();
	std::cerr << "2" << std::endl;
	data->getVertexStructure();
	std::cerr << "3" << std::endl;

	std::string poolId = entry->getVertexStructure().getStringRepresentation() + "|" + char('A' + (usage & 0xFF)) + char('A' + ((usage >> 8) & 0xFF));

	PoolCollectionMap::iterator iter = poolCollections.find(poolId);
	if (iter != poolCollections.end())
		iter->second->free(entry);
	else
		throw TGen::RuntimeException("VertexCache::removeVertexData", "entry poolid not associated with any pool collection! this is insane!");	
	*/
}

void TGen::Engine::VertexCache::lockDynamicPools() {
	
}

void TGen::Engine::VertexCache::unlockDynamicPools() {
	
}

TGen::VertexDataSource & TGen::Engine::VertexCache::getDataSource() const {
	return dataSource;
}

// TODO: man sorterar inte på VertexData sen, utan vilken vb som den ingår i...
/*
	Per material -> per vb -> per ib?    <--
	Eller:
	Per vb -> per material?
 
 */

TGen::Engine::PoolCollection::PoolCollection(const TGen::VertexStructure & vertstruct, ushort usage, TGen::Engine::VertexCache & cache)
	: vertstruct(vertstruct)
	, usage(usage)
	, cache(cache)
{
	sizeLeft = vertstruct.getSize() * 3 * 100000;
	buffer = cache.getDataSource().createVertexData(vertstruct, sizeLeft, usage);
	dataAt = NULL;
}

TGen::Engine::VertexCacheEntry * TGen::Engine::PoolCollection::alloc(uint size) {
	if (size < sizeLeft) {
		sizeLeft -= size;
		
		void * dataPos = dataAt;
		dataAt = reinterpret_cast<char *>(dataAt) + size;
		std::cout << "space at " << dataPos << " for " << size << " bytes!" << std::endl;
		
		return new VertexCacheEntry(cache, size, usage, buffer, dataPos,  reinterpret_cast<uint>(dataPos) / vertstruct.getSize());
	}
	else {
		throw TGen::RuntimeException("PoolCollection::alloc", "no space left!");
	}
	
	// TODO: openglrenderer::drawINdexedPrimitive borkar offset
	
	return NULL;
}

void TGen::Engine::PoolCollection::free(TGen::Engine::VertexCacheEntry * entry) {
	
}


