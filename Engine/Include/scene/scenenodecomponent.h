/*
 *  scenenodecomponent.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENENODECOMPONENT_H
#define _TGEN_ENGINE_SCENENODECOMPONENT_H

#include "component.h"

namespace TGen {
	class SceneNode;
	
	namespace Engine {
		class SceneNodeComponent : public TGen::Engine::Component {
		public:
			SceneNodeComponent(const std::string & name, TGen::SceneNode * sceneNode);
			~SceneNodeComponent();
			
			TGen::SceneNode * getSceneNode();
			
		private:
			TGen::SceneNode * sceneNode;
		};
		
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENENODECOMPONENT_H
