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
			class EquipmentNode;
		}
		
		class WeaponInterface;
		
		namespace Controller {
			class FirstPerson : public TGen::Engine::PlayerController {
			public:
				FirstPerson(const std::string & name, scalar deltaPlane, scalar jumpForce, scalar jumpTime);
				~FirstPerson();
				
				void trigger(TGen::Engine::TriggerContext & context, TriggerMode mode);
				void link(const TGen::Engine::ComponentLinker & linker);
				void update(scalar dt);

				void setDeltaPlane(scalar speed);
				void setWeaponLink(const std::string & weaponName);
				void setEquipment(const std::string & equipmentName);
				void setUsePhysics(bool usePhysics);
				void setAirControl(scalar airControl);
				void setView(const std::string & viewName);
				void setControl(const std::string & controlName);
								
				
				// world object interface
				TGen::Vector3 getVelocity() const;
				TGen::Vector3 getPosition() const;
				TGen::Rotation getOrientation() const;		
				void setOrientation(const TGen::Rotation & orientation);
				
			private:
				void linkGlobally(const TGen::Engine::ComponentLinker & linker);
				
				TGen::Engine::Scene::Node * node;		// IMPL
				TGen::SceneNode * viewNode;
				TGen::Engine::Physics::Body * controlBody;
				TGen::Engine::WeaponInterface * weapon;
				TGen::Engine::Scene::EquipmentNode * equipment;
				
				bool usePhysics, primaryFire, secondaryFire;
				scalar airTime;
				
				std::string controlName, viewName, weaponName, equipmentName;
				scalar orientX, orientY;			
				scalar deltaPlane;
				scalar jumpTime, jumpForce, airControl;
			};
			
			
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FIRSTPERSONCONTROLLER_H

