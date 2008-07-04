/*
 *  equipmentnode.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENE_EQUIPMENTNODE_H
#define _TGEN_ENGINE_SCENE_EQUIPMENTNODE_H

#include <tgen_renderer.h>
#include "componentinterfaces.h"

namespace TGen {
	namespace Engine {
		class EntityList;
		class Entity;
		
		namespace Scene {
			class EquipmentData;
			
			class EquipmentNode : public TGen::SceneNode, public TGen::Engine::WeaponInterface {
			public:
				EquipmentNode(const std::string & name);
				~EquipmentNode();
				
				void linkGlobally(TGen::Engine::EntityList & list, TGen::Engine::Entity & entity);
				void setInitialChild(const std::string & initialChild);
				void traverse(const TGen::SceneNode::Walker & walker);
				void setEquipmentData(const std::string & name, TGen::Engine::Scene::EquipmentData * data);
				
				void changeEquipmentRelative(int relative);
				void changeEquipmentAbsolute(int absolute);
				
				void beginFire(int mode);
				void endFire(int mode);
				
			private:
				void changeEquipment(TGen::Engine::Scene::EquipmentData * equipment);
				
				std::string initialChild;
				TGen::Engine::Scene::EquipmentData * visibleEquipment, * previousEquipment;
				
				typedef std::map<std::string, TGen::Engine::Scene::EquipmentData *> EquipmentMap;
				EquipmentMap equipmentData;
				
				std::vector<TGen::Engine::Scene::EquipmentData *> equipmentSorted;
			};
			
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_EQUIPMENTNODE_H

