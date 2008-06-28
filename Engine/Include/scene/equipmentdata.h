/*
 *  equipmentdata.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/28/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENE_EQUIPMENTDATA_H
#define _TGEN_ENGINE_SCENE_EQUIPMENTDATA_H

#include <string>

namespace TGen {
	class SceneNode;
	
	namespace Engine {
		class EntityList;
		class Entity;
		
		namespace Inventory {
			class Inventory;
			class Weapon;
		}
		
		namespace Scene {
			class EquipmentNode;
			
			class EquipmentData {
			public:
				EquipmentData(const std::string & name);
				
				void setRequires(const std::string & requiresName);
				void setWeapon(const std::string & weaponName);
				void setIndex(int index);
				int getIndex() const;
				
				void linkGlobally(TGen::Engine::Scene::EquipmentNode * node, TGen::Engine::EntityList & list, TGen::Engine::Entity & entity);
				bool inInventory();
				
				TGen::SceneNode * getNode();
				TGen::Engine::Inventory::Weapon * getWeapon();
				
			private:
				std::string name, requiresName, weaponName;
				TGen::SceneNode * node;
				TGen::Engine::Inventory::Inventory * inventory;
				TGen::Engine::Inventory::Weapon * weapon;
				int index;
			};
			
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_EQUIPMENTDATA_H

