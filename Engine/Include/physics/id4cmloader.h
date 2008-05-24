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
#include <tgen_math.h>
#include "physics/id4cmpolygon.h"
#include <ode/ode.h>

namespace TGen {
	namespace Engine {
		class Filesystem;
		
		namespace Physics {
			class Id4CMGeom;
			
			class Id4CMLoader {
			public:
				Id4CMLoader(TGen::Engine::Filesystem & filesystem);
				
				TGen::Engine::Physics::Id4CMGeom * createGeom(const std::string & name, const std::string & path, dSpaceID space);
				
			private:
				void parseGlobalBlock();
				void parseCollisionModelBlock(const std::string & name);
				void parseVertexBlock();
				void parseEdgeBlock();
				void parseNodeBlock();
				void parsePolygonBlock();
				
				void step();
				void checkEOS();
				void stepAndCheck();

				void getContour(std::vector<uint> & contour, const TGen::Engine::Physics::Id4CMPolygon & polygon);
					
				TGen::TokenStream tokens;
				TGen::TokenStream::TokenList::iterator currentToken, endIter;
				
				TGen::Engine::Filesystem & filesystem;
				
				
				// stuff that maybe should be elsewhere
				std::vector<TGen::Vector3> vertices;
				std::vector<std::pair<uint, uint> > edges;
				std::vector<Id4CMPolygon> polygons;
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ID4CMLOADER_H

