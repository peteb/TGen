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

namespace TGen {
	namespace Engine {
		class EntityList;
		class Entity;
		
		namespace Scene {
			class EquipmentNode : public TGen::SceneNode {
			public:
				EquipmentNode(const std::string & name);
				~EquipmentNode();
				
				void linkGlobally(TGen::Engine::EntityList & list, TGen::Engine::Entity & entity);
				void setInitialChild(const std::string & initialChild);
				void traverse(const TGen::SceneNode::Walker & walker);
				
			private:
				std::string initialChild;
				TGen::SceneNode * visibleNode;
			};
			
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_EQUIPMENTNODE_H

