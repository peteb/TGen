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

/*
	En samling pools per material, ie
	material1
		32k
		64k
		128k
 
	material2
		32k
		64k
		128k
 
	material1 with animation time 4		// det här borde nog inte skötas av pools eftersom inte många material delar samma anim time förutom de som
													// kör global time
		32k
		64k
		128k

 
	- Wastear MASSOR med minne, ett material som kanske bara används på ett ställe får ändå en massa pools
 
	
	
	En stor pool:
		32k
		64k
		128k
 
	Sen en lista med alla material som pekar på pool entries
	material1
		entry 5, 8, 15
 
	material2
		entry 5
 
	ELLER entries per poolsize kanske
 
	material1
		32k
			entry 5, 8
		64k
			entry 15
 
 
	+ Slösar mindre utrymme än metod #1
	- Behöver en extra look-up för material->pool entry
	
	Hur gör man med vertex decls? Kan man köra olika vdecls i samma poolsamling? Nej... går inte som det är nu, eftersom man skapar en VB med en vertex decl.
	Kanske går med OpenGL dock, men nej. bäst är att köra 1 poolsamling = 1 vb/ib = 1 vertex decl
	
	I båda fallen ska gränssnittet från en pool manager ~vara:
		void * allocPoolEntry(size, material)
	
		
	För att allokera en pool entry:
		Pool * pool = getPool(CeilPowerOfTwo(size));
		Entry * entry = pool->entries.pop();
 
	För att freea en entry:
		entry->pool->entries.push(entry);
	
	Strukturen som hanterar entries ska initieras med alla entries som får plats
		push lägger till en entry sist
		pop tar den entry som ligger först
		FIFO alltså, med snabb push sist och snabb pop först
		std::queue
		Men varken queue eller deque garanterar linjärt minne... vilket vi vill ha!
		Eller så är varje pool entry en pekare till riktiga minnet
		
*/
