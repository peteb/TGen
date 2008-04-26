/*
 *  arcball.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ARCBALL_H
#define _TGEN_ENGINE_ARCBALL_H

#include "playercontroller.h"

namespace TGen {
	namespace Engine {
		class Entity;
		
		namespace Controller {
			class Arcball : public TGen::Engine::PlayerController {
			public:	
				Arcball(const std::string & name);
				~Arcball();
				
				void linkLocally(TGen::Engine::Entity & entity);
				void update(scalar dt);
				
			};
			
		} // !Controller
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ARCBALL_H

