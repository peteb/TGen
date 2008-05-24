/*
 *  id4cmgeom.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ID4CMGEOM_H
#define _TGEN_ENGINE_ID4CMGEOM_H

#include "physics/geom.h"
#include "physics/meshdecl.h"

#include <vector>

namespace TGen {
	namespace Engine {
		namespace Physics {
			class Id4CMGeom : public TGen::Engine::Physics::Geom {
			public:
				Id4CMGeom(const std::string & name);
				~Id4CMGeom();
				
				void setGeom(dGeomID id);
				
				std::vector<StridedVertex> vertexData;
				std::vector<StridedNormal> normalData;
				std::vector<StridedTriangle> indexData;
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ID4CMGEOM_H

