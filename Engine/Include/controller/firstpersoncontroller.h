/*
 *  firstpersoncontroller.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_FIRSTPERSONCONTROLLER_H
#define _TGEN_ENGINE_FIRSTPERSONCONTROLLER_H

#include "playercontroller.h"
//#include "4p5dofcam.h"

namespace TGen {
	namespace Engine {
		class Entity;
		
		namespace Physics {
			class Body;
		}
		
		namespace Controller {
			class FirstPerson : public TGen::Engine::PlayerController {
			public:
				FirstPerson(const std::string & name, const std::string & control, const std::string & view, bool usePhysics);
				~FirstPerson();
				
				void linkLocally(TGen::Engine::Entity & entity);
				void update(scalar dt);
				
			private:
				TGen::SceneNode * node;		// IMPL
				TGen::SceneNode * viewNode;
				TGen::Engine::Physics::Body * controlBody;
				
				bool usePhysics;
				
				std::string control, view;
				scalar orientX, orientY;			
			};
			
			
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FIRSTPERSONCONTROLLER_H

