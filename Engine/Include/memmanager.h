/*
 *  memmanager.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MEMMANAGER_H
#define _TGEN_ENGINE_MEMMANAGER_H

//#include "mempool.h"
#include <map>

namespace TGen {
	namespace Engine {
		class MemoryManager {
		public:
			MemoryManager();
			~MemoryManager();
			
			template<typename T>
			T * alloc() {
				return static_cast<T *>(malloc(sizeof(T)));
			}
			
			template<typename T>
			void free(T * entry) {
				free(entry);
			}
			
		private:
			void * doAlloc(size_t size);
												
			//typedef std::map<int, TGen::Engine::MemoryPool *> PoolMap;
			//PoolMap pools;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MEMMANAGER_H
