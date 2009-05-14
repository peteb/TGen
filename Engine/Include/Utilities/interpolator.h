/*
 *  interpolator.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/3/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_UTILITIES_INTERPOLATOR_H
#define _TGEN_ENGINE_UTILITIES_INTERPOLATOR_H

#include "component.h"
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		namespace Utilities {
			class Interpolator : public TGen::Engine::Component {
			public:	
				Interpolator(const std::string & name);
				
				void link(const TGen::Engine::ComponentLinker & linker);
				void update(scalar dt);
				
			};
			
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_INTERPOLATOR_H

