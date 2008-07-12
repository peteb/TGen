/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_INVENTORY_SUBSYSTEM_H
#define _TGEN_ENGINE_INVENTORY_SUBSYSTEM_H

#include "../subsystem.h"
#include "item.h"

namespace TGen {
	namespace Engine {
		namespace Inventory {
			class FireMode;
			class Weapon;
			
			class Subsystem : public TGen::Engine::Subsystem {
			public:
				Subsystem();
				~Subsystem();
				
				TGen::Engine::Component * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				void update(scalar dt);
				
			private:
				TGen::Engine::Inventory::FireMode * createFireMode(const TGen::PropertyTree & properties);
				
				TGen::Engine::Inventory::Item * createItem(const TGen::PropertyTree & properties);
				
				std::vector<TGen::Engine::Inventory::Weapon *> weapons;
			}; 
		} // !Inventory
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_INVENTORY_SUBSYSTEM_H

