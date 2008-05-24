/*
 *  meshgeom.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/18/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MESHGEOM_H
#define _TGEN_ENGINE_MESHGEOM_H

#include "physics/geom.h"
#include "physics/meshdecl.h"

namespace TGen {
	class PropertyTree;
	
	namespace Engine {
		namespace Physics {			
			class MeshGeom : public TGen::Engine::Physics::Geom {
			public:
				MeshGeom(const std::string & name, dSpaceID space, const TGen::PropertyTree & vertices, const TGen::PropertyTree & indices, const TGen::PropertyTree & normals);
				
				
			private:
				std::auto_ptr<StridedVertex> vertexData;
				std::auto_ptr<StridedNormal> normalData;
				std::auto_ptr<StridedTriangle> indexData;
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MESHGEOM_H

