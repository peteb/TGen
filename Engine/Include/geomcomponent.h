/*
 *  geomcomponent.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_GEOMCOMPONENT_H
#define _TGEN_ENGINE_GEOMCOMPONENT_H

#include "component.h"
#include <ode/ode.h>

namespace TGen {
	namespace Engine {
		class GeomComponent : public TGen::Engine::Component {
		public:
			GeomComponent(const std::string & name);
			virtual ~GeomComponent();
			
			float getFriction() const;
			void setFriction(float friction);
			void linkLocally(TGen::Engine::Entity & entity);
			void linkGlobally(TGen::Engine::EntityList & entities);

		protected:
			void setGeomId(dGeomID id);
			
		private:
			float friction;
			dGeomID geomId;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_GEOMCOMPONENT_H

