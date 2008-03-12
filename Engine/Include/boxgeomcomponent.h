/*
 *  boxgeomcomponent.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/2/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_BOXGEOMCOMPONENT_H
#define _TGEN_ENGINE_BOXGEOMCOMPONENT_H

#include "geomcomponent.h"
#include <ode/ode.h>

namespace TGen {
	class Vector3;
	
	namespace Engine {
		class Entity;
		class EntityList;
		
		class BoxGeomComponent : public TGen::Engine::GeomComponent {
		public:
			BoxGeomComponent(const std::string & name, const TGen::Vector3 & size, dSpaceID space);
			~BoxGeomComponent();
			
			void linkLocally(TGen::Engine::Entity & entity);
			void linkGlobally(TGen::Engine::EntityList & entities);
			
		private:
			dGeomID geomId;			
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_BOXGEOMCOMPONENT_H

