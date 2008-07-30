/*
 *  noderecipe.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENE_NODERECIPE_H
#define _TGEN_ENGINE_SCENE_NODERECIPE_H

#include "componentrecipe.h"
#include "node.h"

namespace TGen {
	namespace Engine {
		namespace Scene {	
			class Subsystem;
			
			class NodeRecipe : public TGen::Engine::ComponentRecipe {
			public:
				NodeRecipe(const std::string & name, TGen::SceneNode * prototypeNode, TGen::Engine::Scene::Subsystem & subsystem);
				~NodeRecipe();
				
				void link(const TGen::Engine::ComponentLinker & linker, TGen::Engine::EntityRecipe & entity);
				void setLink(const std::string & linkName);

				TGen::Engine::Scene::Node * createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing);

				TGen::SceneNode * getPrototypeNode();
				
			private:
				TGen::Engine::Scene::Subsystem & subsystem;	// the component should be added to the subsystem when created
				TGen::Engine::Scene::Node * parent;
				TGen::SceneNode * prototypeNode;
				
				std::string linkName;
				bool refersSelfEntity;
			};
			
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_NODERECIPE_H

