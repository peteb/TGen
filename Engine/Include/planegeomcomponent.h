/*
 *  planegeomcomponent.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PLANEGEOMCOMPONENT_H
#define _TGEN_ENGINE_PLANEGEOMCOMPONENT_H

#include "geomcomponent.h"

namespace TGen {
	class Plane3;
	
	namespace Engine {
		class PlaneGeomComponent : public TGen::Engine::GeomComponent {
		public:
			PlaneGeomComponent(const std::string & name, const TGen::Plane3 & plane, dSpaceID space);
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PLANEGECOMPONENT_H
