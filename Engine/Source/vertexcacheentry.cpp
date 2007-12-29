/*
 *  vertexcacheentry.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vertexcacheentry.h"

TGen::Engine::VertexCacheEntry2::VertexCacheEntry2(TGen::VertexDataSource & creator, uint size, 
																	ushort usage, TGen::VertexData * buffer, void * entryAt, uint readAt)
	: TGen::VertexData(creator, size, usage, entryAt, readAt)
	, buffer(buffer)
{
}

TGen::Engine::VertexCacheEntry2::~VertexCacheEntry2() {
	std::cout << "22222 remove this cache entry" << std::endl;
}

void * TGen::Engine::VertexCacheEntry2::lock(ushort flags) {
	std::cout << "22222 VERTEX CACHE ENTRY LOCK!!!" << std::endl;
	
	void * bufferLockedAt = buffer->lock(flags);
	
	return reinterpret_cast<char *>(bufferLockedAt) + reinterpret_cast<uint>(writeOffset);
}

void TGen::Engine::VertexCacheEntry2::unlock() {
	std::cout << "22222 VERTEX CACHE ENTRY UNLOCK!!!" << std::endl;
	
	buffer->unlock();
}

void TGen::Engine::VertexCacheEntry2::bufferData(const void * data, uint size, void * offset) {
	std::cout << "222222 VERTEX CACHE ENTRY BUFFERDATA!!!" << std::endl;
	buffer->bufferData(data, size, reinterpret_cast<char *>(writeOffset) + reinterpret_cast<uint>(offset));
}

void TGen::Engine::VertexCacheEntry2::bindShaderVariable(int id, const TGen::ShaderVariable & var) {
	buffer->bindShaderVariable(id, var);
}

TGen::VertexStructure & TGen::Engine::VertexCacheEntry2::getVertexStructure() {
	return buffer->getVertexStructure();
}

TGen::VertexData * TGen::Engine::VertexCacheEntry2::operator * () {
	return buffer;
}
