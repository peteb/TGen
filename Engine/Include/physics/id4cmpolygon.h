/*
 *  id4cmpolygon.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ID4CMPOLYGON_H
#define _TGEN_ENGINE_ID4CMPOLYGON_H

#include <tgen_math.h>

namespace TGen {
	namespace Engine {
		namespace Physics {
			class Id4CMPolygon {
			public:
				Id4CMPolygon();
				
				void addEdge(uint num);
				
				TGen::Plane3 plane;
				TGen::Vector3 min, max;
				std::string materialName;
				
			private:
				std::vector<uint> edges;
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ID4CMPOLYGON_H

