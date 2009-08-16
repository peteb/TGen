/*
 *  id3cmloader.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/15/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include <string>
#include <ode/ode.h>

namespace TGen {
	class VertexTransformer;
	
	namespace Engine {
		class Filesystem;
		
		namespace Physics {
			class Id3CMGeom;
			
			class Id3CMLoader {
			public:
				Id3CMLoader(TGen::Engine::Filesystem & filesystem);
				
				TGen::Engine::Physics::Id3CMGeom * createGeom(const std::string & name, const std::string & path, const TGen::VertexTransformer & transformer, dSpaceID space);
			
			private:
				TGen::Engine::Filesystem & filesystem;
				
			};
			
		} // !Physics
	} // !Engine
} // !TGen
