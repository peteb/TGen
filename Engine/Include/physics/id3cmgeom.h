/*
 *  id3cmgeom.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/15/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "physics/geom.h"
#include "physics/meshdecl.h"

#include <vector>

namespace TGen {
	namespace Engine {
		namespace Physics {
			class Id3CMGeom : public TGen::Engine::Physics::Geom {
			public:
				Id3CMGeom(const std::string & name);
				~Id3CMGeom();
				
				void setGeom(dGeomID id);
				
				std::vector<StridedVertex> vertexData;
				std::vector<StridedNormal> normalData;
				std::vector<StridedTriangle> indexData;
				
				std::vector<StridedVertex> vertices;
				std::vector<int> indices;
			};
			
		} // !Physics
	} // !Engine
} // !TGen