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

namespace TGen {
	namespace Engine {
		class GeomComponent : public TGen::Engine::Component {
		public:
			GeomComponent(const std::string & name);

			float getFriction() const;
			void setFriction(float friction);
			
		private:
			float friction;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_GEOMCOMPONENT_H

