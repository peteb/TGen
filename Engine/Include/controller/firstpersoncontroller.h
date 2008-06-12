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
		
		namespace Scene {
			class Node;
		}
		
		class WeaponInterface;
		
		namespace Controller {
			class FirstPerson : public TGen::Engine::PlayerController {
			public:
				FirstPerson(const std::string & name, const std::string & control, const std::string & view, bool usePhysics, scalar deltaPlane, scalar jumpForce, scalar jumpTime, scalar airControl);
				~FirstPerson();
				
				void linkLocally(TGen::Engine::Entity & entity);
				void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);
				void update(scalar dt);
				void setDeltaPlane(scalar speed);
				void setWeaponLink(const std::string & weaponName);
				
				TGen::Vector3 getVelocity() const;
				TGen::Vector3 getPosition() const;
				TGen::Rotation getOrientation() const;		
				
			private:
				TGen::Engine::Scene::Node * node;		// IMPL
				TGen::SceneNode * viewNode;
				TGen::Engine::Physics::Body * controlBody;
				TGen::Engine::WeaponInterface * weapon;
				
				bool usePhysics, primaryFire, secondaryFire;
				scalar airTime;
				
				std::string control, view, weaponName;
				scalar orientX, orientY;			
				scalar deltaPlane;
				scalar jumpTime, jumpForce, airControl;
			};
			
			
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FIRSTPERSONCONTROLLER_H

