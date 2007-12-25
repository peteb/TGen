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

TGen::Engine::VertexCacheEntry::VertexCacheEntry(TGen::VertexDataSource & creator, uint size, ushort usage)
	: TGen::VertexData(creator, size, usage)
{

}


void * TGen::Engine::VertexCacheEntry::lock(ushort flags) {
	return NULL;
}

void TGen::Engine::VertexCacheEntry::unlock() {
	
}

void TGen::Engine::VertexCacheEntry::bufferData(const void * data, uint size, void * offset) {
	
}

void TGen::Engine::VertexCacheEntry::bindShaderVariable(int id, const TGen::ShaderVariable & var) {
	
}

TGen::VertexStructure & TGen::Engine::VertexCacheEntry::getVertexStructure() {
	
}


TGen::Engine::VertexCache::VertexCache(TGen::VertexDataSource & dataSource, TGen::Engine::StandardLogs & logs) 
	: dataSource(dataSource)
	, logs(logs)
{
	
	logs.info["vcache+"] << "initialized" << TGen::endl;
}

TGen::Engine::VertexCache::~VertexCache() {
	logs.info["vcache-"] << "shutting down..." << TGen::endl;
}

TGen::VertexData * TGen::Engine::VertexCache::createVertexData(const VertexStructure & vertstruct, uint size, ushort usage) {
	logs.info["vcache"] << "someone's asking for " << size << " bytes!" << TGen::endl;
	
	return dataSource.createVertexData(vertstruct, size, usage);
}

void TGen::Engine::VertexCache::removeVertexData(VertexData * data) {
	TGen::Engine::VertexCacheEntry * entry = static_cast<TGen::Engine::VertexCacheEntry *>(data);
	
}

void TGen::Engine::VertexCache::lockDynamicPools() {
	
}

void TGen::Engine::VertexCache::unlockDynamicPools() {
	
}



