/*
 *  resourcecomponent.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/20/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_RESOURCECOMPONENT_H
#define _TGEN_ENGINE_RESOURCECOMPONENT_H

#include "component.h"
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class ResourceComponent : public TGen::Engine::Component {
		public:
			ResourceComponent(const std::string & name) : TGen::Engine::Component(name, true) {}
			
			virtual ~ResourceComponent() {}

			virtual void * getData() const abstract;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_RESOURCECOMPONENT_H

