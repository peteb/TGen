/*
 *  geom.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_GEOM_H
#define _TGEN_ENGINE_PHYSICS_GEOM_H

#include "component.h"
#include <ode/ode.h>

namespace TGen {
	namespace Engine {
		namespace Physics {
			class Geom : public TGen::Engine::Component {
			public:
				Geom(const std::string & name, const std::string & bodyComponent = "physBody");
				virtual ~Geom();
				
				float getFriction() const;
				void setFriction(float friction);
				void setBodyComponent(const std::string & body);
				
				void linkLocally(TGen::Engine::Entity & entity);
				void linkGlobally(TGen::Engine::EntityList & entities);
				
			protected:
				void setGeomId(dGeomID id);
				
			private:
				std::string bodyComponent;
				float friction;
				dGeomID geomId;
			};
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICS_GEOM_H

