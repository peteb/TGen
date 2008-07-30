/*
 *  weapon.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_INVENTORY_WEAPON_H
#define _TGEN_ENGINE_INVENTORY_WEAPON_H

#include "component.h"
#include "componentinterfaces.h"

namespace TGen {
	namespace Engine {
		class EntityRecipe;
		
		namespace Inventory {
			class FireMode;
			
			class Weapon : public TGen::Engine::Component, public TGen::Engine::WeaponInterface {
			public:
				Weapon(const std::string & name);
				virtual ~Weapon();
				
				void link(const TGen::Engine::ComponentLinker & linker);
				//void setAmmoSpawn(const std::string & ammoSpawn);
				
				void update(scalar dt);
				void beginFire(int mode);
				void endFire(int mode);
				
				void setFireMode(int num, TGen::Engine::Inventory::FireMode * mode);
				
			private:
				//TGen::Engine::EntityRecipe * prototype;
				//TGen::Engine::WorldObject * ammoSpawn;
				
				std::string ammoSpawnName;
				std::vector<FireMode *> fireModes;
			};
			
		} // !Inventory
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_INVENTORY_WEAPON_H

