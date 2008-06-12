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
			class NodeRecipe : public TGen::Engine::ComponentRecipe {
			public:
				NodeRecipe(const std::string & name, TGen::SceneNode * prototypeNode);
				~NodeRecipe();
				
				TGen::Engine::Scene::Node * createComponent();
				void setOrigin(const TGen::Vector3 & origin);
				void setOrientation(const TGen::Rotation & orientation);
				void setModel(const std::string & name, const std::string & materialName);
				void setLinkWith(const std::string & linkWith);
				
				void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::EntityRecipe & entity);
				
			private:
				TGen::Vector3 origin;
				TGen::Rotation orientation;
				TGen::Engine::Scene::Node * parent;
				TGen::SceneNode * prototypeNode;
				
				std::string modelName, materialName;
				std::string linkWith;
			};
			
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_NODERECIPE_H

