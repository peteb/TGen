/*
 *  q3maploader.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_Q3BSP_Q3MAPLOADER_H
#define _TGEN_ENGINE_Q3BSP_Q3MAPLOADER_H

#include <string>

namespace TGen {
	class VertexTransformer;

	namespace Engine {
		class StandardLogs;
		class Filesystem;
		class Q3Map;
		
		class Q3MapLoader {
		public:	
			Q3MapLoader(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem);
			
			TGen::Engine::Q3Map * createMap(const std::string & name, const std::string & filename, const TGen::VertexTransformer & transformer);
			
		private:
			TGen::Engine::StandardLogs & logs;
			TGen::Engine::Filesystem & filesystem;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_Q3BSP_Q3MAPLOADER_H

