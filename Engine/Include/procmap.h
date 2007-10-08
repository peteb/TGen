/*
 *  procmap.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PROCMAP_H
#define _TGEN_ENGINE_PROCMAP_H

#include <string>

namespace TGen {
	namespace Engine {
		class ProcMap {
		public:
			ProcMap();
			~ProcMap();
			
			void load(const std::string & filename);
			void clear();
			
			
			
			
			
		};
		
		
		
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PROCMAP_H
