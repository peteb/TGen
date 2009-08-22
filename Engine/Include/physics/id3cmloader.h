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
#include <vector>

namespace TGen {
	class VertexTransformer;
	
	namespace Engine {
		class Filesystem;
		class Component;
		
		namespace Physics {
			class Id3CMGeom;
			class Geom;
			
			class Id3CMLoader {
			public:
				Id3CMLoader(TGen::Engine::Filesystem & filesystem);
				
				std::vector<TGen::Engine::Physics::Geom *> fillGeoms(const std::string & name, const std::string & path, const TGen::VertexTransformer & transformer, dSpaceID space);
			
			private:
				TGen::Engine::Filesystem & filesystem;
				
			};
			
		} // !Physics
	} // !Engine
} // !TGen
