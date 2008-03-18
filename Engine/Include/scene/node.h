/*
 *  node.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENE_NODE_H
#define _TGEN_ENGINE_SCENE_NODE_H

#include "component.h"

namespace TGen {
	class SceneNode;
	
	namespace Engine {
		namespace Scene {
			class Node : public TGen::Engine::Component {
			public:
				Node(const std::string & name, TGen::SceneNode * sceneNode);
				~Node();
				
				TGen::SceneNode * getSceneNode();
				
			private:
				TGen::SceneNode * sceneNode;
			};
		
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_NODE_H
