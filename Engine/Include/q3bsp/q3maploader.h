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
	class InputStream;
	
	namespace Engine {
		class StandardLogs;
		class Q3Map;
		class Q3BspFile;
		
		class Q3MapLoader {
		public:	
			Q3MapLoader(TGen::Engine::StandardLogs & logs);
			
			TGen::Engine::Q3Map * createMap(const std::string & name, TGen::InputStream & source, const TGen::VertexTransformer & transformer);
			
			
			
			
		private:
			void loadBspFile(TGen::InputStream & source, TGen::Engine::Q3BspFile & file);
			
			TGen::Engine::StandardLogs & logs;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_Q3BSP_Q3MAPLOADER_H

