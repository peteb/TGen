/*
 *  mempool.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MEMPOOL_H
#define _TGEN_ENGINE_MEMPOOL_H

namespace TGen {
	namespace Engine {
		template<int entrySize>
		class MemoryPool {
			struct Entry {
				uchar data[entrySize];
				bool free;
			};
			
		public:
			MemoryPool(size_t preAlloc) {
				entries = new Entry[preAlloc];
				entriesAlloced = preAlloc;
			}
			
			~MemoryPool() {
				delete [] entries;
			}
			
			void * alloc() {
				for (int i = 0; i < entriesAlloced; ++i) {
					if (entries[i].free)
						return &entries[i];
				}
			}
			
			void free(void * entry) {
				static_cast<Entry *>(entry)->free = true;
			}
			
			void getEntrySize() {
				return sizeof(Entry);
			}
			
		private:
			Entry * entries;
			int entriesAlloced;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MEMPOOL_H
