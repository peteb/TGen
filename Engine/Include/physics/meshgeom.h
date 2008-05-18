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

namespace TGen {
	class PropertyTree;
	
	namespace Engine {
		namespace Physics {			
			
			struct StridedVertex {		// TODO: ska flyttas in i en gemensam header för alla som vill använda
				dVector3 vertex;
			};
			
			struct StridedTriangle {
				int indices[3];
			};
			
			
			class MeshGeom : public TGen::Engine::Physics::Geom {
			public:
				MeshGeom(const std::string & name, dSpaceID space, const TGen::PropertyTree & vertices, const TGen::PropertyTree & indices);
				
				
			private:
				std::auto_ptr<StridedVertex> vertexData;
				std::auto_ptr<StridedTriangle> indexData;
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MESHGEOM_H

