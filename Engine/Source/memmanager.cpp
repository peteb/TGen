/*
 *  memmanager.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "memmanager.h"

TGen::Engine::MemoryManager::MemoryManager() {

}

TGen::Engine::MemoryManager::~MemoryManager() {
	//for (PoolMap::iterator iter = pools.begin(); iter != pools.end(); ++iter)
//		delete iter->second;
}

void * TGen::Engine::MemoryManager::doAlloc(size_t size) {
	/*PoolMap::iterator iter = pools.find(size);
	if (iter == pools.end())
		throw TGen::RuntimeException("MemoryManager::doAlloc", "sized pool does not exist, size: ") << size;
	
	return iter->second->alloc(size);*/
	
	return NULL;
}
