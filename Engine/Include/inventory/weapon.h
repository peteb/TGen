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
		namespace Inventory {
			class Weapon : public TGen::Engine::Component, public TGen::Engine::WeaponInterface {
			public:
				Weapon(const std::string & name);

				void beginFire(int mode);
				void endFire(int mode);
			};
			
		} // !Inventory
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_INVENTORY_WEAPON_H

