/*
 *  firemode.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/28/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_FIREMODE_H
#define _TGEN_ENGINE_FIREMODE_H

#include <tgen_core.h>
#include <tgen_math.h>

namespace TGen {
	namespace Engine {
		class EntityRecipe;
		class WorldObject;
		class EntityList;
		class Entity;
		
		namespace Inventory {
			class Inventory;
			
			class FireMode {
			public:
				FireMode();
				
				void beginFire();
				void endFire();
				void update(scalar dt);
				void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);
				
				void setOrigin(const std::string & originName);
				void setProjectile(const std::string & projectileName);
				void setRequires(const std::string & requiresName);
				void setRate(scalar rate);
				void setSpread(const TGen::Vector2 & spread);
				
			private:
				void fire();
				
				bool doFire;
				scalar frequency, timeSinceFire;
				std::string originName, projectileName, requiresName;
				TGen::Vector2 spread;
				
				TGen::Engine::EntityRecipe * projectile;
				TGen::Engine::WorldObject * spawnOrigin;
				TGen::Engine::Inventory::Inventory * inventory;
			};
			
		} // !Inventory
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FIREMODE_H

