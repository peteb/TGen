/*
 *  maploader.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/29/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MAPLOADER_H
#define _TGEN_ENGINE_MAPLOADER_H

#include <tgen_math.h>
#include "mapsurface.h"

namespace TGen {
	class VertexTransformer;
	
	namespace Engine {
		class Map;
		class StandardLogs;
		class Filesystem;
		class MapModel;
		
		class MapLoader {
		public:
			MapLoader(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem);
			
			TGen::Engine::Map * createMap(const std::string & name, const std::string & filename, const TGen::VertexTransformer & transformer);
			
		private:
			void parseGlobalBlock(TGen::Engine::Map * map);
			void parseNodesBlock(TGen::Engine::Map * map);
			void parseIAPBlock(TGen::Engine::Map * map);

			TGen::Engine::MapModel * parseModelBlock(TGen::Engine::Map * map);
			TGen::Engine::MapSurface * parseSurfaceBlock();
			TGen::Engine::MapSurface::VertexDecl::Type parseVertex();
			
			void step();
			void checkEOS();			
			void stepAndCheck();
			
			TGen::Engine::StandardLogs & logs;
			TGen::Engine::Filesystem & filesystem;

			TGen::TokenStream tokens;
			TGen::TokenStream::TokenList::iterator currentToken, endIter;
		};
		

	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAPLOADER_H
