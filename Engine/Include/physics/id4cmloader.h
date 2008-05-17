/*
 *  id4cmloader.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ID4CMLOADER_H
#define _TGEN_ENGINE_ID4CMLOADER_H

#include <string>
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class Filesystem;
		
		namespace Physics {
			class Id4CMGeom;
			
			class Id4CMLoader {
			public:
				Id4CMLoader(TGen::Engine::Filesystem & filesystem);
				
				TGen::Engine::Physics::Id4CMGeom * createGeom(const std::string & path);
				
			private:
				TGen::TokenStream tokens;
				TGen::TokenStream::TokenList::iterator currentToken, endIter;
				
				TGen::Engine::Filesystem & filesystem;
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ID4CMLOADER_H

